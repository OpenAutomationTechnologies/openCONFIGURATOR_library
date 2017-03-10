/************************************************************************
\file CProcessImageGenerator.cpp
\brief Implementation of the Class CProcessImageGenerator
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
#include "CProcessImageGenerator.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;


CProcessImageGenerator::CProcessImageGenerator() : ProcessImageGenerator(),
	processImageStream()
{}

CProcessImageGenerator::~CProcessImageGenerator()
{}

CProcessImageGenerator& CProcessImageGenerator::GetInstance()
{
	static CProcessImageGenerator instance;
	return instance;
}

const std::string CProcessImageGenerator::Generate(std::uint8_t nodeid, std::shared_ptr<Network> network)
{
	std::uint32_t size = 0;
	std::stringstream fullProcessImage;
	std::shared_ptr<BaseNode> node;
	Result res = network->GetBaseNode(nodeid, node);
	if (!res.IsSuccessful())
	{
		boost::format formatter(kMsgNonExistingNode[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% (std::uint32_t) nodeid;
		LOG_FATAL() << formatter.str();
		return "";
	}

	fullProcessImage << this->WriteCHeader(network->GetNetworkId(), node);
	size = this->WriteCProcessImage(Direction::RX, node);
	if (size != 0)
	{
		fullProcessImage << this->WriteCOutputSizeHeader(size);
		fullProcessImage << this->processImageStream.str();
	}
	size = this->WriteCProcessImage(Direction::TX, node);
	if (size != 0)
	{
		fullProcessImage << this->WriteCInputSizeHeader(size);
		fullProcessImage << this->processImageStream.str();
	}
	fullProcessImage << "#endif" << std::endl;
	return fullProcessImage.str();
}

const std::string CProcessImageGenerator::PrintChannel(const std::string& name, const IEC_Datatype& type, const std::uint32_t size, const std::uint32_t, const boost::optional<std::uint32_t>&)
{
    IEC_Datatype usedType = type;
    
    if(type == IEC_Datatype::BITSTRING){
        usedType = size > 32 ? IEC_Datatype::DWORD : IEC_Datatype::LWORD;
    }
    
	std::stringstream channel;
	switch (usedType)
	{
		case IEC_Datatype::SINT:
		case IEC_Datatype::INT:
		case IEC_Datatype::DINT:
			channel << "\tsigned ";
			channel << Utilities::ClearModuleParameterUuid(name) << ":";
			channel << std::dec << size;
			channel << ";" << std::endl;
			break;
        case IEC_Datatype::LINT:
            channel << "\tINT64 ";
			channel << Utilities::ClearModuleParameterUuid(name) << ":";
			channel << std::dec << size;
			channel << ";" << std::endl;
			break;
		case IEC_Datatype::USINT:
		case IEC_Datatype::UINT:
		case IEC_Datatype::UDINT:
		case IEC_Datatype::BOOL:
		case IEC_Datatype::BYTE:
		case IEC_Datatype::_CHAR:
		case IEC_Datatype::WORD:
		case IEC_Datatype::DWORD:
			channel << "\tunsigned ";
			channel << Utilities::ClearModuleParameterUuid(name) << ":";
			channel << std::dec << size;
			channel << ";" << std::endl;
			break;
        case IEC_Datatype::ULINT:
        case IEC_Datatype::LWORD:
            channel << "\tUINT64 ";
			channel << Utilities::ClearModuleParameterUuid(name) << ":";
			channel << std::dec << size;
			channel << ";" << std::endl;
            break;
		case IEC_Datatype::REAL:
			channel << "\tfloat ";
			channel << Utilities::ClearModuleParameterUuid(name);
			channel << ";" << std::endl;
			break;
		case IEC_Datatype::LREAL:
			channel << "\tdouble ";
			channel << Utilities::ClearModuleParameterUuid(name);
			channel << ";" << std::endl;
			break;
		case IEC_Datatype::STRING:
		case IEC_Datatype::WSTRING:
		case IEC_Datatype::UNDEFINED:
		default:
			channel << "";
			break;
	}
	return channel.str();
}

const std::string CProcessImageGenerator::WriteCHeader(const std::string& projName, const std::shared_ptr<BaseNode>& node)
{
	std::ostringstream dateTime;
	std::stringstream header;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	//No deletion needed : http://stackoverflow.com/questions/17779660/who-is-responsible-for-deleting-the-facet
	boost::posix_time::time_facet* const f = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	dateTime.imbue(std::locale(dateTime.getloc(), f));
	dateTime << now;

	header << "/*" << std::endl << "* This file was autogenerated by openCONFIGURATOR-";
	header << kVersionMajor << "." << kVersionMinor << "." << kVersionFix << "_" << kReleaseType << " on " << dateTime.str() << std::endl;
	header << "* Project: " << projName << std::endl;
	header << "* Application process for " << node->GetName() << "(";
	header << std::dec << (std::uint32_t) node->GetNodeId();
	header << ")" << std::endl << "*/" << std::endl;
	header << "#ifndef XAP_h" << std::endl;
	header << "#define XAP_h" << std::endl << std::endl;

	return header.str();
}

const std::string CProcessImageGenerator::WriteCOutputSizeHeader(std::uint32_t size)
{
	std::stringstream outputHeader;
	outputHeader << "#define COMPUTED_PI_OUT_SIZE " << size / 8 << std::endl;
	outputHeader << "typedef struct" << std::endl;
	outputHeader << "{" << std::endl;
	return outputHeader.str();
}

const std::string CProcessImageGenerator::WriteCInputSizeHeader(std::uint32_t size)
{
	std::stringstream inputHeader;
	inputHeader << "#define COMPUTED_PI_IN_SIZE " << size / 8 << std::endl;
	inputHeader << "typedef struct" << std::endl;
	inputHeader << "{" << std::endl;
	return inputHeader.str();
}

std::uint32_t CProcessImageGenerator::WriteCProcessImage(const Direction& dir, const std::shared_ptr<BaseNode>& node)
{
	this->processImageStream.str(std::string());
	std::vector<std::shared_ptr<BaseProcessImageObject>> processImage;
	std::uint16_t paddingCount = 1;
	std::uint32_t piTotalSize = 0;
	std::uint32_t piSize = 0;

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
			// Align the PI to 8 bit, 16 bit and 32 bit
			if (piEntry->GetSize() % 8 == 0 || piEntry->GetSize() % 16 == 0 || piEntry->GetSize() % 32 == 0)
			{
				if (piSize % piEntry->GetSize() != 0)
				{
					std::uint32_t paddingSize = piEntry->GetSize() - (piSize % piEntry->GetSize());
					piSize += paddingSize;
					while (paddingSize != 0)
					{
						std::stringstream paddingVarName;
						paddingVarName << "PADDING_VAR_" << paddingCount;
						if (paddingSize >= 32)
						{
							this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 32, 0, boost::optional<std::uint32_t>());
							paddingSize -= 32;
						}
						else if (paddingSize >= 16)
						{
							this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 16, 0, boost::optional<std::uint32_t>());
							paddingSize -= 16;
						}
						else if (paddingSize >= 8)
						{
							this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 8, 0, boost::optional<std::uint32_t>());
							paddingSize -= 8;
						}
						else
						{
							this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, paddingSize, 0, boost::optional<std::uint32_t>());
							paddingSize = 0;
						}
						paddingCount++;
					}
				}
			}
			this->processImageStream << PrintChannel(piEntry->GetName(), piEntry->GetDataType(), piEntry->GetSize(), piEntry->GetPiOffset(), piEntry->GetBitOffset());
			piSize += piEntry->GetSize();
		}

		//Align the complete PI to 32bit
		if (piSize % 32 != 0)
		{
			std::uint32_t paddingSize = 32 - (piSize % 32);

			piSize += paddingSize;
			while (paddingSize != 0)
			{
				std::stringstream paddingVarName;
				paddingVarName << "PADDING_VAR_" << paddingCount;
				if (paddingSize >= 32)
				{
					this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 32, 0, boost::optional<std::uint32_t>());
					paddingSize -= 32;
				}
				else if (paddingSize >= 16)
				{
					this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 16, 0, boost::optional<std::uint32_t>());
					paddingSize -= 16;
				}
				else if (paddingSize >= 8)
				{
					this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, 8, 0, boost::optional<std::uint32_t>());
					paddingSize -= 8;
				}
				else
				{
					this->processImageStream << PrintChannel(paddingVarName.str(), IEC_Datatype::USINT, paddingSize, 0, boost::optional<std::uint32_t>());
					paddingSize = 0;
				}
				paddingCount++;
			}
		}

		if (dir == Direction::RX)
			this->processImageStream << "} PI_OUT;" << std::endl << std::endl;
		else if (dir == Direction::TX)
			this->processImageStream << "} PI_IN;" << std::endl << std::endl;
	}
	return piSize;
}
