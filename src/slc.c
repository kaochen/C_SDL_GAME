/*C_SDL_Game is a simple SDL Game write to practice C
Authors: kao_chen
Copyright (C) 2016 kao_chen <kaochen2[at]gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef SLC_C
#define SLC_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../inc/slc.h"
#include "../inc/const.h"

//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>

int readXML(void)
{
    xmlDocPtr doc;
    xmlNodePtr root;

    /* Open SLC/XML file */
    doc = xmlParseFile("levels/12_Blocks.slc");
    if (doc == NULL)
    {
        fprintf(stderr, "XML Document not valid\n");
        return 0;
    }

    // Start XPath
	xmlXPathInit();
	// Create a context
	xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
	if (ctxt == NULL) {
	    fprintf(stderr, "Error creating the context XPath\n");
	    exit(-1);
	}
	// Read expression with XPath
	xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST "/SokobanLevels/Title/text()", ctxt);
	if (xpathRes == NULL) {
	    fprintf(stderr, "Error on the XPath expression\n");
	    exit(-1);
	}
	// Show the result
	if (xpathRes->type == XPATH_NODESET) {
	    int i;
	    printf("Title: ");
	    for (i = 0; i < xpathRes->nodesetval->nodeNr; i++) {
		xmlNodePtr n = xpathRes->nodesetval->nodeTab[i];
		if (n->type == XML_TEXT_NODE || n->type == XML_CDATA_SECTION_NODE) {
		    fprintf(stderr, "%s\n", n->content);
		}
	    }
	}
	// free memorie
	xmlFreeDoc(doc);
	xmlXPathFreeObject(xpathRes);
	xmlXPathFreeContext(ctxt);
  	return 1;
}
#endif
