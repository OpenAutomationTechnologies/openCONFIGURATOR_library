/************************************************************************
\file LoggingConfiguration.cpp
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
#include "LoggingConfiguration.h"

using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace CoreConfiguration
			{

				BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

				/**
				\brief	Custom formatter for the boost.log "Scope" attribute.

						Implementation of a custom formatter to output file- and line number
						information for boost.log's "Scope" attribute.
				\author pufferd, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				struct scope_list_formatter
				{
						typedef void result_type;
						typedef logging::attributes::named_scope::value_type scope_stack;

						explicit scope_list_formatter(const logging::attribute_name& name) : name(name)
						{
						}

						void operator()(const logging::record_view& rec, logging::formatting_ostream& strm) const
						{
							// We need to acquire the attribute value from the log record
							logging::visit<scope_stack>(
							    name,
							    rec.attribute_values(),
							    boost::bind(&scope_list_formatter::format, _1, boost::ref(strm))
							);
						}

					private:
						logging::attribute_name name;

						// This is where our custom formatting takes place
						static void format(const scope_stack& scopes, logging::formatting_ostream& strm)
						{
							if (scopes.empty())
							{
								strm << "(scope empty)";
							}
							else
							{
								logging::attributes::named_scope_list::const_reference scope_list = scopes.back();
								std::string file(scope_list.file_name.str());
								size_t pos = file.find_last_of(kPathSeparator);

								strm << file.substr(pos + 1)
								     << ":"
								     << scopes.back().line
								     << " ("
								     << scopes.back().scope_name
								     << ")";
							}
						}
				};

				/**
				\brief	A custom Scope formatter factory.

						Custom formatter factory for a Scope attribute, using the custom
						formatter scope_list_formatter.
				\author pufferd, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class scope_formatter_factory : public logging::formatter_factory<char>
				{
					public:
						/*
						 * This function creates a formatter for the Scopes attribute.
						 * It effectively associates the attribute with the scope_list_formatter class
						 */
						formatter_type create_formatter(
						    const logging::attribute_name& attr_name, const args_map&)
						{
							return formatter_type(scope_list_formatter(attr_name));
						}
				};

				Result LoggingConfiguration::InitConfiguration(const std::string& configuration)
				{
					try
					{
						logging::register_simple_formatter_factory< logging::trivial::severity_level, char >("Severity");
						logging::register_simple_filter_factory< logging::trivial::severity_level, char >("Severity");

						logging::register_simple_formatter_factory< unsigned int, char >("Line");
						logging::register_simple_filter_factory< unsigned int, char >("Line");

						logging::register_formatter_factory("Scope", boost::make_shared<scope_formatter_factory>());

						std::istringstream configStream(configuration);
						logging::init_from_stream(configStream);

						logging::core::get()->add_global_attribute("Scope", logging::attributes::named_scope());
						logging::add_common_attributes();

						sev_logger::get();
						return Result();
					}
					catch (const boost::exception& ex)
					{
						LOG_FATAL() << boost::diagnostic_information(ex);
						return Result(ErrorCode::UNHANDLED_EXCEPTION, boost::diagnostic_information(ex));
					}
				}

				Result LoggingConfiguration::InitEclipseConfiguration(const std::string& loggingPath)
				{
					try
					{
						logging::add_common_attributes();

						logging::add_file_log
						(
						    keywords::file_name = loggingPath + "org.epsg.openconfigurator.%N.log",
						    keywords::format =
						        (
						            expr::stream
						            /* << "!ENTRY org.epsg.openconfigurator.core " */
						            << "["
						            << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S")
						            << "] "
						            << expr::if_(logging::trivial::severity == logging::trivial::info)
						            [
						                expr::stream <<  "[INFO] "
						            ]
						            << expr::if_(logging::trivial::severity == logging::trivial::warning)
						            [
						                expr::stream << "[WARNING] "
						            ]
						            << expr::if_(logging::trivial::severity == logging::trivial::error)
						            [
						                expr::stream << "[ERROR] "
						            ]
						            << expr::if_(logging::trivial::severity == logging::trivial::fatal)
						            [
						                expr::stream << "[FATAL] "
						            ]
						            /*<< " 0 "*/
						            /*<< "!MESSAGE " */
						            << expr::smessage
						        ),
						    keywords::auto_flush = "true",
						    keywords::open_mode = std::ios_base::trunc,
						    keywords::filter = logging::trivial::severity >= logging::trivial::warning

						);

						sev_logger::get();
						return Result();
					}
					catch (const boost::exception& ex)
					{
						LOG_FATAL() << boost::diagnostic_information(ex);
						return Result(ErrorCode::UNHANDLED_EXCEPTION, boost::diagnostic_information(ex));
					}
				}
			}
		}
	}
}