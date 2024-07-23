#include "onppgnb.hpp"

void onpp::gnb::OMNetGNBListerner::onConnected
(
    app::NodeType subjectType, 
    const std::string &subjectId, 
    app::NodeType objectType,
    const std::string &objectId
) 
{
    m_logger->info(
        "onConnected -> Subject Type: {}, Subject ID: {}, Object Type: {}, Object ID: {}", 
        subjectType,
        subjectId,
        objectType,
        objectId
    );
}

void onpp::gnb::OMNetGNBListerner::onReceive
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
        subjectType, 
        subjectId,
        objectType,
        objectId,
        connectionType
    );
}

void onpp::gnb::OMNetGNBListerner::onSend
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
        subjectType,
        subjectId,
        objectType,
        objectId,
        connectionType
    );
}

void onpp::gnb::OMNetGNBListerner::onSwitch 
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
        subjectType,
        subjectId,
        stateType,
        fromState,
        toState
    );
}