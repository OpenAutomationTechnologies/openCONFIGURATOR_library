/************************************************************************
\file XmlProcessImageGenerator.cpp
\brief Implementation of the Class XmlProcessImageGenerator
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
#include "XmlProcessImageGenerator.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

XmlProcessImageGenerator::XmlProcessImageGenerator() : ProcessImageGenerator(),
	processImageStream()
{}

XmlProcessImageGenerator::~XmlProcessImageGenerator()
{}

XmlProcessImageGenerator& XmlProcessImageGenerator::GetInstance()
{
	static XmlProcessImageGenerator instance;
	return instance;
}

const std::string XmlProcessImageGenerator::Generate(std::uint8_t nodeid, std::shared_ptr<Network> network)
{
	//clear PI stream
	this->processImageStream.str(std::string());

	WriteXMLHeader();
	std::shared_ptr<BaseNode> node;
	network->GetBaseNode(nodeid, node);

	this->processImageStream << "<!-- Application process for " << node->GetName() << "(";
	this->processImageStream << std::dec << (std::uint32_t) node->GetNodeId();
	this->processImageStream << ") -->" << std::endl;

	this->processImageStream << "<ApplicationProcess>" << std::endl;

	if (node->GetReceiveProcessImageSize() != 0)
	{
		WriteXMLOutputSizeHeader(node);
		for (auto& receive : node->GetReceiveProcessImage())
		{
			this->processImageStream << PrintChannel(receive->GetName(), receive->GetDataType(), receive->GetSize(), receive->GetPiOffset(), receive->GetBitOffset());
		}
		this->processImageStream << "\t</ProcessImage>" << std::endl;
	}

	if (node->GetTransmitProcessImageSize() != 0)
	{
		WriteXMLInputSizeHeader(node);
		for (auto& transmit : node->GetTransmitProcessImage())
		{
			this->processImageStream << PrintChannel(transmit->GetName(), transmit->GetDataType(), transmit->GetSize(), transmit->GetPiOffset(), transmit->GetBitOffset());
		}
		this->processImageStream << "\t</ProcessImage>" << std::endl;
	}

	this->processImageStream << "</ApplicationProcess>" << std::endl;
	return this->processImageStream.str();
}

const std::string XmlProcessImageGenerator::PrintChannel(const std::string& name, const IEC_Datatype datatype, const std::uint32_t size, const std::uint32_t piOffset, const boost::optional<std::uint32_t>& bitOffset)
{
	std::stringstream channel;

	channel << "\t\t<Channel";
	channel << " name=\"" + name + "\"";
	channel << " dataType=\"" + IECDatatypeValues[(std::int8_t) datatype] + "\"";
	channel << " dataSize=\"";
	channel << std::dec << size;
	channel << "\"";
	channel << " PIOffset=\"" << IntToHex<std::uint32_t>(piOffset, 4, "0x") << "\"";
	if (bitOffset.is_initialized() && size < 8)
		channel << " BitOffset=\"" + IntToHex<std::uint32_t>(bitOffset.get(), 2, "0x") + "\"";
	channel << "/>" << std::endl;
	return channel.str();
}

void XmlProcessImageGenerator::WriteXMLHeader()
{
	std::ostringstream dateTime;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	//No deletion needed : http://stackoverflow.com/questions/17779660/who-is-responsible-for-deleting-the-facet
	boost::posix_time::time_facet* const f = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	dateTime.imbue(std::locale(dateTime.getloc(), f));
	dateTime << now;

	this->processImageStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	this->processImageStream << "<!-- This file was autogenerated by openCONFIGURATOR-";
	this->processImageStream << kVersionMajor << "." << kVersionMinor << "." << kVersionFix << " on " << dateTime.str() << " -->" << std::endl;
}

void XmlProcessImageGenerator::WriteXMLOutputSizeHeader(const std::shared_ptr<BaseNode>& node)
{
	this->processImageStream << "\t<ProcessImage type=\"output\" size=\"" << node->GetReceiveProcessImageSize() / 8 << "\">" << std::endl;
}

void XmlProcessImageGenerator::WriteXMLInputSizeHeader(const std::shared_ptr<BaseNode>& node)
{
	this->processImageStream << "\t<ProcessImage type=\"input\" size=\"" << node->GetTransmitProcessImageSize() / 8 << "\">" << std::endl;
}
