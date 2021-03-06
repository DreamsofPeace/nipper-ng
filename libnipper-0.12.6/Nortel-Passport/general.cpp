/***************************************************************************
 *   Nipper - The network infrastructure parser                            *
 *   Copyright (C) 2006 - 2008 by Ian Ventura-Whiting                      *
 *   fizz@titania.co.uk                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version. However, there are a number of    *
 *   exceptions listed below.                                              *
 *                                                                         *
 *   EXCEPTIONS TO THE GPL License:                                        *
 *      1. This code cannot be used as part of a commercial product. If a  *
 *         commercial license is required, contact fizz@titania.co.uk for  *
 *         licensing information.                                          *
 *      2. Any code that makes use of this library MUST display the        *
 *         returned text of the Nipper::getVersion() method. Since the     *
 *         text returned includes the version details and copyright of     *
 *         the library, the sugested place to show it would be with the    *
 *         callers "about program" information.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

// Includes...
#include <cstdlib>
#include <cstring>

#include "../globaldefs.h"
#include "../device/common/configline.h"
#include "device.h"
#include "general.h"


// Initialisation...
PassportGeneral::PassportGeneral()
{
}


int PassportGeneral::processDeviceConfig(Device *device, ConfigLine *command, char *line, int lineSize)
{
	// Variables...
	int tempInt = 0;

	// Box type...
	if ((strcmp(command->part(tempInt), "#") == 0) && (strcmp(command->part(tempInt + 1), "box") == 0) && (strcmp(command->part(tempInt + 2), "type") == 0) && (strcmp(command->part(tempInt + 3), ":") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sBox Type Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		deviceModel.assign(command->part(tempInt + 4));
	}

	// Software Version...
	else if ((strcmp(command->part(tempInt), "#") == 0) && (strcmp(command->part(tempInt + 1), "software") == 0) && (strcmp(command->part(tempInt + 2), "version") == 0) && (strcmp(command->part(tempInt + 3), ":") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sSoftware Version Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		version.assign(command->part(tempInt + 4));
	}

	// Monitor Version...
	else if ((strcmp(command->part(tempInt), "#") == 0) && (strcmp(command->part(tempInt + 1), "monitor") == 0) && (strcmp(command->part(tempInt + 2), "version") == 0) && (strcmp(command->part(tempInt + 3), ":") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sMonitor Version Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		monitorVersion.assign(command->part(tempInt + 4));
	}

	// Modules
	else if ((strcmp(command->part(tempInt), "#") == 0) && (strcmp(command->part(tempInt + 1), "Slot") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sModule Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		tempInt += 2;
		if (strcmp(command->part(tempInt + 1), "--") != 0)
			addDeviceModule(atoi(command->part(tempInt)), command->part(tempInt + 1), strstr(line, command->part(tempInt + 4)));
		else
			addDeviceModule(atoi(command->part(tempInt)), i18n("Empty"));
	}

	else if ((strcmp(command->part(tempInt), "sys") == 0) && (strcmp(command->part(tempInt + 1), "set") == 0) && (strcmp(command->part(tempInt + 2), "location") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sLocation Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		location.assign(command->part(tempInt + 3));
	}

	else if ((strcmp(command->part(tempInt), "sys") == 0) && (strcmp(command->part(tempInt + 1), "set") == 0) && (strcmp(command->part(tempInt + 2), "contact") == 0))
	{
		if (device->config->reportFormat == Config::Debug)
			printf("%sContact Line:%s %s\n", device->config->COL_GREEN, device->config->COL_RESET, line);
		contact.assign(command->part(tempInt + 3));
	}

	return 0;
}


int PassportGeneral::processDefaults(Device *device)
{
	// Split version numbers...
	if (!version.empty())
	{
		versionMajor = atoi(version.c_str());
		versionMinor = atoi(strchr(version.c_str(), '.') + 1);
		versionRevision = atoi(strchr(strchr(version.c_str(), '.') + 1, '.') + 1);
		versionTweak = atoi(strchr(strchr(strchr(version.c_str(), '.') + 1, '.') + 1, '.') + 1);
	}

	return 0;
}


int PassportGeneral::generateConfigSpecificReport(Device *device)
{
	// Variables...
	Device::configReportStruct *configReportPointer = 0;
	Device::paragraphStruct *paragraphPointer = 0;
	int errorCode = 0;

	// Get Config Report Section...
	configReportPointer = device->getConfigSection("CONFIG-GENERAL");

	// Get Paragraph Pointer...
	paragraphPointer = device->getTableParagraphPointer("CONFIG-GENERAL-TABLE");

	// Monitor version...
	if (!monitorVersion.empty())
	{
		device->addTableData(paragraphPointer->table, i18n("Monitor Version"));
		device->addTableData(paragraphPointer->table, monitorVersion.c_str());
	}

	return errorCode;
}


int PassportGeneral::generateSecuritySpecificReport(Device *device)
{
	// Variables...
	int errorCode = 0;
/*
	// Service Password Encryption...
	if (servicePasswordEncryption == off)
	{
		securityIssuePointer = device->addSecurityIssue();
		securityIssuePointer->title.assign(i18n("Service Password Encryption Disabled"));
		securityIssuePointer->reference.assign("IOS.PASSENCR.1");
		securityIssuePointer->overallRating = 8;
		securityIssuePointer->impactRating = 9;
		securityIssuePointer->easeRating = 2;
		securityIssuePointer->fixRating = 1;
		paragraphPointer = device->addParagraph(securityIssuePointer, Device::Finding);
		paragraphPointer->paragraph.assign(i18n("Cisco service passwords by default are stored in their clear text form rather than being encrypted. However, it is possible to have these passwords stored using the reversible Cisco type-7 encryption. *COMPANY* determined that the service password encryption that helps to provide a basic level of encryption for passwords that otherwise would be stored in clear text was disabled on the Cisco device *DEVICENAME*."));
		paragraphPointer = device->addParagraph(securityIssuePointer, Device::Impact);
		paragraphPointer->paragraph.assign(i18n("The Cisco type-7 password encryption provides little in the way of protection against an attacker who has access to the configuration file. However, the Cisco type-7 password encryption would provide a level of protect against a malicious user who had managed to briefly view a devices configuration."));
		paragraphPointer = device->addParagraph(securityIssuePointer, Device::Ease);
		paragraphPointer->paragraph.assign(i18n("If a malicious user were able to glance a password from a devices configuration they could use it to gain a level of access to *DEVICENAME*. If the passwords were encrypted using the Cisco type-7 routine, a malicious user that had access to the devices configuration would have to decode the password first. Tools are widely available on the Internet that can reverse Cisco type-7 passwords."));
		paragraphPointer = device->addParagraph(securityIssuePointer, Device::Recommendation);
		paragraphPointer->paragraph.assign(i18n("*COMPANY* recommends that the Cisco password encryption service be enabled. The Cisco password encryption service can be started with the following Cisco *ABBREV*IOS*-ABBREV* command:"));
		paragraphPointer = device->addParagraph(securityIssuePointer, Device::Recommendation);
		paragraphPointer->paragraph.assign(i18n("*CODE**COMMAND*service password-encryption*-COMMAND**-CODE*"));
		securityIssuePointer->conLine.assign(i18n("service password encryption was disabled"));
		securityIssuePointer->recLine.assign(i18n("enable the service password encryption"));
	}*/
	return errorCode;
}
