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
	//clear PI stream
	this->processImageStream.str(std::string());

	std::shared_ptr<BaseNode> node;
	network->GetBaseNode(nodeid, node);

	WriteNETHeader(node);

	this->WriteNETProcessImage(Direction::RX, node);
	this->WriteNETProcessImage(Direction::TX, node);

	this->processImageStream << "}" << std::endl;

	return this->processImageStream.str();
}

void NetProcessImageGenerator::WriteNETProcessImage(const Direction dir, const std::shared_ptr<BaseNode>& node)
{
	std::vector<std::shared_ptr<BaseProcessImageObject>> processImage;
	std::uint32_t piSize = 0;
	std::uint32_t piOffset = 0;
	std::uint16_t paddingCount = 1;
	std::uint32_t bitCount = 0;
	std::string startName;

	if (dir == Direction::RX)
	{
		piSize = node->GetReceiveProcessImageSize();
		processImage = node->GetReceiveProcessImage();

	}
	else if (dir == Direction::TX)
	{
		piSize = node->GetTransmitProcessImageSize();
		processImage = node->GetTransmitProcessImage();
	}

	if (piSize != 0)
	{
		if (dir == Direction::RX)
			WriteNETOutputSizeHeader(node);
		else if (dir == Direction::TX)
			WriteNETInputSizeHeader(node);

		for (auto& piEntry : processImage)
		{
			if (piEntry->GetDataType() == IEC_Datatype::BITSTRING)
			{
				if (bitCount == 0)
					startName = piEntry->GetName();
				bitCount += piEntry->GetSize();

				if (bitCount == 8)
				{
					startName = startName + "_to_" + piEntry->GetName();
					this->processImageStream << "\t\t[FieldOffset(" << piEntry->GetPiOffset() << ")]" << std::endl;
					this->processImageStream << PrintChannel(startName, piEntry->GetDataType(), piEntry->GetSize(), piEntry->GetPiOffset(), piEntry->GetBitOffset());
					bitCount = 0;
					startName = "";
					piOffset++;
				}
				continue;
			}
			else
			{
				this->processImageStream << "\t\t[FieldOffset(" << piEntry->GetPiOffset() << ")]" << std::endl;
				this->processImageStream << PrintChannel(piEntry->GetName(), piEntry->GetDataType(), piEntry->GetSize(), piEntry->GetPiOffset(), piEntry->GetBitOffset());
				piOffset = piEntry->GetPiOffset() + piEntry->GetSize() / 8;
			}
		}
		if (piSize % 32 != 0)
		{
			std::uint32_t totalSize = piSize;
			while (totalSize % 32 != 0)
			{
				std::stringstream convert;
				convert << "PADDING_VAR_" << paddingCount;
				this->processImageStream << "\t\t[FieldOffset(" << piOffset << ")]" << std::endl;
				this->processImageStream << PrintChannel(convert.str(), IEC_Datatype::USINT, 8, piOffset, boost::optional<std::uint32_t>());
				totalSize += 8;
				paddingCount++;
				piOffset++;

			}
		}
		this->processImageStream << "\t}" << std::endl << std::endl;
	}
}

const std::string NetProcessImageGenerator::PrintChannel(const std::string& name, const IEC_Datatype dt, const std::uint32_t, const std::uint32_t, const boost::optional<std::uint32_t>&)
{
	std::stringstream channel;
	channel << "\t\tpublic " << GetNetDatatypeFromIEC(dt) << " " << name << ";" << std::endl;
	return channel.str();
}

void NetProcessImageGenerator::WriteNETHeader(const std::shared_ptr<BaseNode>& node)
{
	std::ostringstream dateTime;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	//No deletion needed : http://stackoverflow.com/questions/17779660/who-is-responsible-for-deleting-the-facet
	boost::posix_time::time_facet* const f = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	dateTime.imbue(std::locale(dateTime.getloc(), f));
	dateTime << now;

	this->processImageStream << "using System;" << std::endl;
	this->processImageStream << "using System.Runtime.InteropServices;" << std::endl;
	this->processImageStream << "/// <summary>" << std::endl;
	this->processImageStream << "/// This file was autogenerated by openCONFIGURATOR-";
	this->processImageStream << kVersionMajor << "." << kVersionMinor << "." << kVersionFix << "_" << kReleaseType << " on " << dateTime.str() << std::endl;
	this->processImageStream << "/// Application process for " << node->GetName() << "(";
	this->processImageStream << std::dec << (uint32_t) node->GetNodeId();
	this->processImageStream << ")" << std::endl;
	this->processImageStream << "/// </summary>" << std::endl << std::endl;
	this->processImageStream << "namespace openPOWERLINK" << std::endl << "{" << std::endl << std::endl;
}

void NetProcessImageGenerator::WriteNETOutputSizeHeader(const std::shared_ptr<BaseNode>& node)
{
	// Pad the size to 4 byte alignment
	std::uint32_t totalSize = node->GetReceiveProcessImageSize();
	if ((totalSize % 32) != 0)
		totalSize += (32 - (totalSize % 32));

	this->processImageStream << "\t/// <summary>" << std::endl;
	this->processImageStream << "\t/// Struct : ProcessImage Out" << std::endl;
	this->processImageStream << "\t/// </summary>" << std::endl;
	this->processImageStream << "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " << totalSize / 8 << ")]" << std::endl;
	this->processImageStream << "\tpublic struct AppProcessImageOut" << std::endl;
	this->processImageStream << "\t{" << std::endl;
}

void NetProcessImageGenerator::WriteNETInputSizeHeader(const std::shared_ptr<BaseNode>& node)
{
	// Pad the size to 4 byte alignment
	std::uint32_t totalSize = node->GetTransmitProcessImageSize();
	if ((totalSize % 32) != 0)
		totalSize += (32 - (totalSize % 32));

	this->processImageStream << "\t/// <summary>" << std::endl;
	this->processImageStream << "\t/// Struct : ProcessImage In" << std::endl;
	this->processImageStream << "\t/// </summary>" << std::endl;
	this->processImageStream << "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " << totalSize / 8 << ")]" << std::endl;
	this->processImageStream << "\tpublic struct AppProcessImageIn" << std::endl;
	this->processImageStream << "\t{" << std::endl;
}


