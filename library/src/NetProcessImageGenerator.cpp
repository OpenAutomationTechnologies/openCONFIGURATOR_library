/************************************************************************
\file NetProcessImageGenerator.cpp
\brief Implementation of the Class NetProcessImageGenerator
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 01-May-2015 12:00:00
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2015, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/
#include "NetProcessImageGenerator.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

NetProcessImageGenerator::NetProcessImageGenerator() : ProcessImageGenerator(),
	processImageStream()
{}

NetProcessImageGenerator::~NetProcessImageGenerator()
{}

NetProcessImageGenerator& NetProcessImageGenerator::GetInstance()
{
	static NetProcessImageGenerator instance;
	return instance;
}

const std::string NetProcessImageGenerator::Generate(std::uint8_t nodeid, std::shared_ptr<Network> network)
{
	std::stringstream fullProcessImage;
	std::uint32_t size = 0;
	std::shared_ptr<BaseNode> node;
	network->GetBaseNode(nodeid, node);

	fullProcessImage << WriteNETHeader(network->GetNetworkId(), node);

	size = this->WriteNETProcessImage(Direction::RX, node);
	if (size != 0)
	{
		fullProcessImage << WriteNETOutputSizeHeader(size);
		fullProcessImage << this->processImageStream.str();
	}

	size = this->WriteNETProcessImage(Direction::TX, node);
	if (size != 0)
	{
		fullProcessImage << WriteNETInputSizeHeader(size);
		fullProcessImage << this->processImageStream.str();
	}
	fullProcessImage << "}" << std::endl;
	return fullProcessImage.str();
}

std::uint32_t NetProcessImageGenerator::WriteNETProcessImage(const Direction dir, const std::shared_ptr<BaseNode>& node)
{
	this->processImageStream.str(std::string());
	std::vector<std::shared_ptr<BaseProcessImageObject>> processImage;
	std::uint32_t piTotalSize = 0;
	std::uint32_t piSize = 0;
	std::uint16_t paddingCount = 1;
	std::uint32_t bitCount = 0;
	std::string startName;

	if (dir == Direction::RX)
	{
		piTotalSize = node->GetReceiveProcessImageSize();
		processImage = node->GetReceiveProcessImage();

	}
	else if (dir == Direction::TX)
	{
		piTotalSize = node->GetTransmitProcessImageSize();
		processImage = node->GetTransmitProcessImage();
	}

	if (piTotalSize != 0)
	{
		for (auto& piEntry : processImage)
		{
			if (piEntry->GetSize() % 8 == 0 || piEntry->GetSize() % 16 == 0 || piEntry->GetSize() % 32 == 0)
			{
				while (piSize % piEntry->GetSize() != 0)
				{
					std::stringstream convert;
					convert << "PADDING_VAR_" << paddingCount;
					this->processImageStream << "\t\t[FieldOffset(" << piSize / 8 << ")]" << std::endl;
					this->processImageStream << PrintChannel(convert.str(), IEC_Datatype::USINT, 8, piSize / 8, boost::optional<std::uint32_t>());
					paddingCount++;
					piSize += 8;
				}
			}

			if (piEntry->GetDataType() == IEC_Datatype::BITSTRING || piEntry->GetDataType() == IEC_Datatype::BOOL)
			{
				if (bitCount == 0)
					startName = piEntry->GetName();
				bitCount += piEntry->GetSize();

				if (bitCount == 8)
				{
					startName = startName + "_to_" + piEntry->GetName();
					this->processImageStream << "\t\t[FieldOffset(" << piSize / 8 << ")]" << std::endl;
					this->processImageStream << PrintChannel(startName, piEntry->GetDataType(), piEntry->GetSize(), piSize / 8, piEntry->GetBitOffset());
					bitCount = 0;
					startName = "";
					piSize += 8;
				}
				continue;
			}
			else
			{
				this->processImageStream << "\t\t[FieldOffset(" << piSize / 8 << ")]" << std::endl;
				this->processImageStream << PrintChannel(piEntry->GetName(), piEntry->GetDataType(), piEntry->GetSize(), piSize / 8, piEntry->GetBitOffset());
				piSize += piEntry->GetSize();
			}
		}

		while (piSize % 32 != 0)
		{
			std::stringstream convert;
			convert << "PADDING_VAR_" << paddingCount;
			this->processImageStream << "\t\t[FieldOffset(" << piSize / 8 << ")]" << std::endl;
			this->processImageStream << PrintChannel(convert.str(), IEC_Datatype::USINT, 8, piSize / 8, boost::optional<std::uint32_t>());
			paddingCount++;
			piSize += 8;
		}

		this->processImageStream << "\t}" << std::endl << std::endl;
	}
	return piSize;
}

const std::string NetProcessImageGenerator::PrintChannel(const std::string& name, const IEC_Datatype dt, const std::uint32_t, const std::uint32_t, const boost::optional<std::uint32_t>&)
{
	std::stringstream channel;
	channel << "\t\tpublic " << GetNetDatatypeFromIEC(dt) << " " << name << ";" << std::endl;
	return channel.str();
}

const std::string NetProcessImageGenerator::WriteNETHeader(const std::string& projName, const std::shared_ptr<BaseNode>& node)
{
	std::ostringstream dateTime;
	std::stringstream header;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	//No deletion needed : http://stackoverflow.com/questions/17779660/who-is-responsible-for-deleting-the-facet
	boost::posix_time::time_facet* const f = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	dateTime.imbue(std::locale(dateTime.getloc(), f));
	dateTime << now;

	header << "using System;" << std::endl;
	header << "using System.Runtime.InteropServices;" << std::endl << std::endl;
	header << "/// <summary>" << std::endl;
	header << "/// This file was autogenerated by openCONFIGURATOR-";
	header << kVersionMajor << "." << kVersionMinor << "." << kVersionFix << "_" << kReleaseType << " on " << dateTime.str() << std::endl;
	header << "/// Project: " << projName << std::endl;
	header << "/// Application process for " << node->GetName() << "(";
	header << std::dec << (uint32_t) node->GetNodeId();
	header << ")" << std::endl;
	header << "/// </summary>" << std::endl << std::endl;
	header << "namespace openPOWERLINK" << std::endl << "{" << std::endl << std::endl;
	return header.str();
}

const std::string NetProcessImageGenerator::WriteNETOutputSizeHeader(std::uint32_t totalSize)
{
	std::stringstream sizeHeader;
	sizeHeader << "\t/// <summary>" << std::endl;
	sizeHeader << "\t/// Struct : ProcessImage Out" << std::endl;
	sizeHeader << "\t/// </summary>" << std::endl;
	sizeHeader << "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " << totalSize / 8 << ")]" << std::endl;
	sizeHeader << "\tpublic struct AppProcessImageOut" << std::endl;
	sizeHeader << "\t{" << std::endl;
	return sizeHeader.str();
}

const std::string NetProcessImageGenerator::WriteNETInputSizeHeader(std::uint32_t totalSize)
{
	std::stringstream sizeHeader;
	sizeHeader << "\t/// <summary>" << std::endl;
	sizeHeader << "\t/// Struct : ProcessImage In" << std::endl;
	sizeHeader << "\t/// </summary>" << std::endl;
	sizeHeader << "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " << totalSize / 8 << ")]" << std::endl;
	sizeHeader << "\tpublic struct AppProcessImageIn" << std::endl;
	sizeHeader << "\t{" << std::endl;
	return sizeHeader.str();
}
