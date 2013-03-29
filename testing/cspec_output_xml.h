/*
 *  cspec_output_xml.h   :  Declaration of the xml output
 *
 * See copyright notice in cspec.h
 *
 */

#ifndef CSPEC_OUTPUT_XML_H
#define CSPEC_OUTPUT_XML_H

#include "cspec_output.h"


CSpecOutputStruct* CSpec_NewOutputXml();

void CSpec_XmlFileOpen(const char *filename, const char *encoding);
void CSpec_XmlFileClose(void);

#endif

