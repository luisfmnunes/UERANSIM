#include "onppue.hpp"

/// @brief Casts the app::NodeType to a string
/// @param type 
/// @return Node Type as a string
static std::string NodeTypeToString(const app::NodeType& type)
{
    switch (type)
    {
    case app::NodeType::AMF:
        return "AMF";
    case app::NodeType::GNB:
        return "GNB";
    case app::NodeType::UE:
        return "UE";
    default:
        return "UNKNOWN";
    }
}


/// @brief Casts the app::ConnectionType to a string
/// @param type 
/// @return Connection Type as a string
static std::string ConnectionTypeToString(const app::ConnectionType& type)
{
    switch (type)
    {
    case app::ConnectionType::SCTP:
        return "SCTP";
    case app::ConnectionType::NGAP:
        return "NGAP";
    default:
        return "UNKNOWN";
    }
}

static std::string StateTypeToString(const app::StateType& type)
{
    switch (type)
    {
    case app::StateType::MM:
        return "MM";
    case app::StateType::MM_SUB:
        return "MM_SUB";
    case app::StateType::RM:
        return "RM";
    case app::StateType::CM:
        return "CM";
    case app::StateType::U5:
        return "U5";
    case app::StateType::RRC:
        return "RRC";
    default:
        return "UNKNOWN";
    }
}

void onpp::ue::OMNetUEListerner::onConnected
(
    app::NodeType subjectType, 
    const std::string &subjectId, 
    app::NodeType objectType,
    const std::string &objectId
) 
{
    m_logger->info(
        "onConnected -> Subject Type: {}, Subject ID: {}, Object Type: {}, Object ID: {}", 
        NodeTypeToString(subjectType),
        subjectId,
        NodeTypeToString(objectType),
        objectId
    );
}

void onpp::ue::OMNetUEListerner::onReceive
(
    app::NodeType subjectType, 
    const std::string &subjectId, 
    app::NodeType objectType,
    const std::string &objectId, 
    app::ConnectionType connectionType, 
    std::string message
) 
{
    m_logger->info(
        "onReceive -> Subject Type: {}, Subject ID: {}, Object Type: {}, Object ID: {}, Connection Type: {}",
        NodeTypeToString(subjectType), 
        subjectId,
        NodeTypeToString(objectType),
        objectId,
        ConnectionTypeToString(connectionType)
    );
}

void onpp::ue::OMNetUEListerner::onSend
(
    app::NodeType subjectType, 
    const std::string &subjectId, 
    app::NodeType objectType,
    const std::string &objectId, 
    app::ConnectionType connectionType, 
    std::string message
) 
{
    m_logger->info(
        "onSend -> Subject Type: {}, Subject ID: {}, Object Type: {}, Object ID: {}, Connection Type: {}",
        NodeTypeToString(subjectType),
        subjectId,
        NodeTypeToString(objectType),
        objectId,
        ConnectionTypeToString(connectionType)
    );
}

void onpp::ue::OMNetUEListerner::onSwitch 
(
    app::NodeType subjectType, 
    const std::string &subjectId, 
    app::StateType stateType,
    const std::string &fromState, 
    const std::string &toState
) 
{
    m_logger->info(
        "onSwitch -> Subject Type: {}, Subject ID: {}, State Type: {}, From State: {}, To State: {}",
        NodeTypeToString(subjectType),
        subjectId,
        StateTypeToString(stateType),
        fromState,
        toState
    );
}