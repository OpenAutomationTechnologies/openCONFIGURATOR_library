/************************************************************************
\file LoggingConfiguration.h
\brief Implementation of the Class LoggingConfiguration
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
#if !defined LOGGING_CONFIGURATION_H
#define LOGGING_CONFIGURATION_H

#include <string>
#include <fstream>

#ifdef _MSC_VER
#pragma warning(push, 0) // Store current warning state and set global warning level 0
#endif

#include <boost/bind.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/format.hpp>

#ifdef _MSC_VER
#pragma warning(pop) // Restore previous warning state
#pragma warning (disable:4503 4714) // Boost-Headers themselves disable this warning, reinstate
#endif

#include "Constants.h"
#include "Result.h"
#include "Language.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief Library configuration related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace CoreConfiguration
			{
				/**
				\brief Handles the boost log configuration.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class LoggingConfiguration
				{
					public:
						virtual ~LoggingConfiguration();

						/**
						\brief Retrieve the singleton instance of the class.
						\return LoggingConfiguration instance
						*/
						static LoggingConfiguration& GetInstance();

						/**
						\brief Initialises the boost logger with the configuration file.
						\param[in] configuration Logging configuration.
						*/
						static IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result InitConfiguration(const std::string& configuration);

						static IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result InitEclipseConfiguration(const std::string& loggingPath);

						const IndustrialNetwork::POWERLINK::Core::CoreConfiguration::Language& GetCurrentLanguage() const;
						void SetCurrentLanguage(const IndustrialNetwork::POWERLINK::Core::CoreConfiguration::Language& lang);

					private:
						//singleton
						LoggingConfiguration();
						LoggingConfiguration(LoggingConfiguration const&);
						void operator=(LoggingConfiguration const&);

						IndustrialNetwork::POWERLINK::Core::CoreConfiguration::Language currentLanguage;
				};

				BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(sev_logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

				#define LOG_TRACE() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::trace)

				#define LOG_DEBUG() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::debug)

				#define LOG_INFO() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::info)

				#define LOG_WARN() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::warning)

				#define LOG_ERROR() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::error)

				#define LOG_FATAL() \
				BOOST_LOG_FUNCTION(); \
				BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::fatal)
			}
		}
	}
}
#endif
