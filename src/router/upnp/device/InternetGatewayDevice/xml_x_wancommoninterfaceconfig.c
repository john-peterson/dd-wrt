/*
 * Broadcom UPnP module, xml_x_wancommoninterfaceconfig.c
 *
 * Copyright (C) 2008, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: xml_x_wancommoninterfaceconfig.c,v 1.5 2007/11/23 09:51:55 Exp $
 */
#include <upnp.h>

char xml_x_wancommoninterfaceconfig[]=
	"<?xml version=\"1.0\"?>\r\n"
	"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\r\n"
	"\t<specVersion>\r\n"
	"\t\t<major>1</major>\r\n"
	"\t\t<minor>0</minor>\r\n"
	"\t</specVersion>\r\n"
	"\t<actionList>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>SetEnabledForInternet</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewEnabledForInternet</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>EnabledForInternet</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetEnabledForInternet</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewEnabledForInternet</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>EnabledForInternet</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetCommonLinkProperties</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewWANAccessType</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>WANAccessType</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLayer1UpstreamMaxBitRate</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>Layer1UpstreamMaxBitRate</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLayer1DownstreamMaxBitRate</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>Layer1DownstreamMaxBitRate</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPhysicalLinkStatus</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PhysicalLinkStatus</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetMaximumActiveConnections</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewMaximumActiveConnections</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>MaximumActiveConnections</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetTotalBytesSent</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewTotalBytesSent</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>TotalBytesSent</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetTotalPacketsSent</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewTotalPacketsSent</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>TotalPacketsSent</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetTotalBytesReceived</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewTotalBytesReceived</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>TotalBytesReceived</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetTotalPacketsReceived</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewTotalPacketsReceived</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>TotalPacketsReceived</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetActiveConnections</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewActiveConnectionIndex</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>NumberOfActiveConnections</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewActiveConnDeviceContainer</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ActiveConnectionDeviceContainer</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewActiveConnectionServiceID</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ActiveConnectionServiceID</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t</actionList>\r\n"
	"\t<serviceStateTable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>WANAccessType</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>DSL</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>POTS</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Cable</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Ethernet</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Other</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>Layer1UpstreamMaxBitRate</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>Layer1DownstreamMaxBitRate</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>PhysicalLinkStatus</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>Up</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Down</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Initializing</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Unavailable</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>EnabledForInternet</name>\r\n"
	"\t\t\t<dataType>boolean</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>MaximumActiveConnections</name>\r\n"
	"\t\t\t<dataType>ui2</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>NumberOfActiveConnections</name>\r\n"
	"\t\t\t<dataType>ui2</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>TotalBytesSent</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>TotalBytesReceived</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>TotalPacketsSent</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>TotalPacketsReceived</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>ActiveConnectionDeviceContainer</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>ActiveConnectionServiceID</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t</serviceStateTable>\r\n"
	"</scpd>\r\n"
	"\r\n";
