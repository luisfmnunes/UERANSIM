#include "task.hpp"
#include "utils.hpp"
#include "encode.hpp"

#include <gnb/gtp/task.hpp>
#include <lib/asn/ngap.hpp>
#include <lib/asn/ngap_msg.hpp>

#include <asn/ngap/ASN_NGAP_NGAP-PDU.h>
#include <asn/ngap/ASN_NGAP_InitiatingMessage.h>
#include <asn/ngap/ASN_NGAP_HandoverCommand.h>
#include <asn/ngap/ASN_NGAP_HandoverCommandTransfer.h>
#include <asn/ngap/ASN_NGAP_ProtocolIE-Container.h>
#include <asn/ngap/ASN_NGAP_PathSwitchRequest.h>
#include <asn/ngap/ASN_NGAP_UserLocationInformationNR.h>
#include <asn/ngap/ASN_NGAP_PDUSessionResourceToBeSwitchedDLItem.h>
#include <asn/ngap/ASN_NGAP_PathSwitchRequestTransfer.h>
#include <asn/ngap/ASN_NGAP_UPTransportLayerInformation.h>
#include <asn/ngap/ASN_NGAP_QosFlowAcceptedList.h>
#include <asn/ngap/ASN_NGAP_QosFlowAcceptedItem.h>
#include <asn/ngap/ASN_NGAP_QosFlowSetupRequestList.h>
#include <asn/ngap/ASN_NGAP_QosFlowSetupRequestItem.h>
#include <asn/ngap/ASN_NGAP_GTPTunnel.h>

namespace nr::gnb{

void NgapTask::sendHandoverCommand(int ueId){

    auto* ue = findUeContext(ueId);
    if(ue == nullptr){
        m_logger->err("Failed to find UE of id [%d] in HandoverCommand", ueId);
        return;
    }
    
    std::vector<ASN_NGAP_HandoverCommandIEs *> pIeC6180P20{};

    for(const auto& pduSess : ue->pduSessions){

        auto *ieHoCT = asn::New<ASN_NGAP_HandoverCommandIEs>();

        auto pduResource = m_base->gtpTask->getTunnelInfo(ueId, pduSess);
        
    }

    auto *pdu = asn::ngap::NewMessagePdu<ASN_NGAP_HandoverCommand>(pIeC6180P20);
}

void NgapTask::handleHandOver(int ueId)
{
    auto *ue = findUeContext(ueId);
    if(ue == nullptr){
        m_logger->err("Failed to find UE of id [%d] in PathSwitchRequest", ueId);
        return;
    }

    auto *amf = findAmfContext(ue->associatedAmfId);
    if(amf == nullptr or amf->state != EAmfState::CONNECTED)
        return;

    m_logger->info("Handling HandOver to AMF [%d]", ue->associatedAmfId);

    auto *pdu = asn::ngap::NewMessagePdu<ASN_NGAP_PathSwitchRequest>({});

    asn::ngap::AddProtocolIeIfUsable(
        *pdu, asn_DEF_ASN_NGAP_AMF_UE_NGAP_ID, ASN_NGAP_ProtocolIE_ID_id_SourceAMF_UE_NGAP_ID,
        ASN_NGAP_Criticality_ignore, [ue](void *mem) {
            auto &id = *reinterpret_cast<ASN_NGAP_AMF_UE_NGAP_ID_t *>(mem);
            asn::SetSigned64(ue->amfUeNgapId, id);
        }
    );

    asn::ngap::AddProtocolIeIfUsable(
        *pdu,
        asn_DEF_ASN_NGAP_UESecurityCapabilities,
        ASN_NGAP_ProtocolIE_ID_id_UESecurityCapabilities,
        ASN_NGAP_Criticality_ignore,
        [this](void *mem){
            auto *sec = reinterpret_cast<ASN_NGAP_UESecurityCapabilities *>(mem);
            asn::SetBitString(sec->nRencryptionAlgorithms, OctetString::FromHex("FFFF"));
                asn::SetBitString(sec->nRintegrityProtectionAlgorithms, OctetString::FromHex("FFFF"));
                asn::SetBitString(sec->eUTRAencryptionAlgorithms, OctetString::FromHex("FFFF"));
                asn::SetBitString(sec->eUTRAintegrityProtectionAlgorithms, OctetString::FromHex("FFFF"));
        }
    );


    asn::ngap::AddProtocolIeIfUsable(
        *pdu,
        asn_DEF_ASN_NGAP_PDUSessionResourceToBeSwitchedDLList, 
        ASN_NGAP_ProtocolIE_ID_id_PDUSessionResourceToBeSwitchedDLList,
        asn::ngap::GetProcedureCriticality(asn::ngap::NgapMessageType::PathSwitchRequest),
        [this, ue, ueId](void* mem){
            auto *PDUList = asn::New<ASN_NGAP_PathSwitchRequestIEs>();

            for(const auto& pduSession : ue->pduSessions){
            //FIXME: 
                auto *pduI = asn::New<ASN_NGAP_PDUSessionResourceToBeSwitchedDLItem>();
                
                // m_logger->info("Tunnel Info: %s", m_base->gtpTask->getTunnelInfo(ueId, pduSession).c_str());

                auto *pathSwitchRequestTransfer = asn::New<ASN_NGAP_PathSwitchRequestTransfer>();
                auto pduResource = m_base->gtpTask->getTunnelInfo(ueId, pduSession);
                
                auto &qosList = pduResource->qosFlows->list;
                for(int iQos = 0; iQos < qosList.count; iQos++){
                    auto *acceptedQosFlowItem = asn::New<ASN_NGAP_QosFlowAcceptedItem>();
                    acceptedQosFlowItem->qosFlowIdentifier = qosList.array[iQos]->qosFlowIdentifier;
                    asn::SequenceAdd(pathSwitchRequestTransfer->qosFlowAcceptedList, acceptedQosFlowItem);
                }

                auto &upInfo = pathSwitchRequestTransfer->dL_NGU_UP_TNLInformation;
                upInfo.present = ASN_NGAP_UPTransportLayerInformation_PR_gTPTunnel;
                upInfo.choice.gTPTunnel = asn::New<ASN_NGAP_GTPTunnel>();
                asn::SetBitString(upInfo.choice.gTPTunnel->transportLayerAddress, pduResource->downTunnel.address);
                asn::SetOctetString4(upInfo.choice.gTPTunnel->gTP_TEID, (octet4)pduResource->downTunnel.teid);

                pduI->pDUSessionID = pduSession;
                OctetString encodedPSRT = ngap_encode::EncodeS(asn_DEF_ASN_NGAP_PathSwitchRequestTransfer, pathSwitchRequestTransfer);
                if(encodedPSRT.length() == 0)
                    m_logger->err("Failed to Encode PathSwitchRequestTransfer");

                asn::SetOctetString(pduI->pathSwitchRequestTransfer, encodedPSRT);
                asn::SequenceAdd(PDUList->value.choice.PDUSessionResourceToBeSwitchedDLList,pduI);

                asn::Free(asn_DEF_ASN_NGAP_PathSwitchRequestTransfer, pathSwitchRequestTransfer);
            }
            *reinterpret_cast<ASN_NGAP_PDUSessionResourceToBeSwitchedDLList_t *>(mem) = PDUList->value.choice.PDUSessionResourceToBeSwitchedDLList;
        }
    );

    asn::ngap::AddProtocolIeIfUsable(
        *pdu, asn_DEF_ASN_NGAP_RAN_UE_NGAP_ID, ASN_NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID,
        ASN_NGAP_Criticality_reject,
    [ue](void *mem) { *reinterpret_cast<ASN_NGAP_RAN_UE_NGAP_ID_t *>(mem) = ue->ranUeNgapId; });

    asn::ngap::AddProtocolIeIfUsable(
        *pdu, asn_DEF_ASN_NGAP_UserLocationInformation, ASN_NGAP_ProtocolIE_ID_id_UserLocationInformation,
        ASN_NGAP_Criticality_ignore, [this](void *mem) {
        auto *loc = reinterpret_cast<ASN_NGAP_UserLocationInformation *>(mem);
        loc->present = ASN_NGAP_UserLocationInformation_PR_userLocationInformationNR;
        loc->choice.userLocationInformationNR = asn::New<ASN_NGAP_UserLocationInformationNR>();

        auto &nr = loc->choice.userLocationInformationNR;
        nr->timeStamp = asn::New<ASN_NGAP_TimeStamp_t>();

        ngap_utils::ToPlmnAsn_Ref(m_base->config->plmn, nr->nR_CGI.pLMNIdentity);
        asn::SetBitStringLong<36>(m_base->config->nci, nr->nR_CGI.nRCellIdentity);
        ngap_utils::ToPlmnAsn_Ref(m_base->config->plmn, nr->tAI.pLMNIdentity);
        asn::SetOctetString3(nr->tAI.tAC, octet3{m_base->config->tac});
        asn::SetOctetString4(*nr->timeStamp, octet4{utils::CurrentTimeStamp().seconds32()});
    });

    sendNgapNonUe(ue->associatedAmfId, pdu);

}

void NgapTask::receivePathSwitchRequestAck(int amfId, ASN_NGAP_PathSwitchRequestAcknowledge *msg){
    m_logger->info("Received PathSwitchRequestAck from AMF[%d]", amfId);

    auto *amf = findAmfContext(amfId);
    if (amf == nullptr)
        return;
}

} // namespace nr::gnb