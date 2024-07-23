#pragma once

#include <gnb/gnb.hpp>
#include <lib/app/monitor.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace onpp::gnb{

class OMNetGNBListerner : public app::INodeListener
{

// Class That implements the INodeListener interface to provide UERANSIM 
// with the ability to listen to the GNB's state changes and messages
// Interconnecting the OMNet++ Simulation and UERANSIM GNB instance

private:
    std::shared_ptr<spdlog::logger> m_logger;

public:

    OMNetGNBListerner() : m_logger(
        std::make_shared<spdlog::logger>(
            "OMNetGNBListerner",
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
        )
    ){}

    virtual void onConnected
    (
        app::NodeType subjectType, 
        const std::string &subjectId, 
        app::NodeType objectType,
        const std::string &objectId
    ) override;

    virtual void onReceive
    (
        app::NodeType subjectType, 
        const std::string &subjectId, 
        app::NodeType objectType,
        const std::string &objectId, 
        app::ConnectionType connectionType, 
        std::string message
    ) override;

    virtual void onSend(
        app::NodeType subjectType, 
        const std::string &subjectId, 
        app::NodeType objectType,
        const std::string &objectId, 
        app::ConnectionType connectionType, 
        std::string message
    ) override;

    virtual void onSwitch
    (
        app::NodeType subjectType, 
        const std::string &subjectId, 
        app::StateType stateType,
        const std::string &fromState, 
        const std::string &toState
    ) override;

}; // class OMNetGNBListerner

} // namespace onpp::gnb
