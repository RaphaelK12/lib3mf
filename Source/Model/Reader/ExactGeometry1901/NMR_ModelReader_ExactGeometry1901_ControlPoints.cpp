/*++

Copyright (C) 2018 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelReaderNode_ExactGeometry1901_NurbsSurface.cpp covers the official 3MF Exact Geometry Extension.

--*/

#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_ControlPoints.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_ControlPoint.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>

namespace NMR {

	CModelReaderNode_ExactGeometry1901_ControlPoints::CModelReaderNode_ExactGeometry1901_ControlPoints(_In_ CModel * pModel, _In_ CModelNurbsSurface * pNurbsSurface, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		if (pNurbsSurface == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModel = pModel;
		m_pWarnings = pWarnings;
		m_pNurbsSurface = pNurbsSurface;

		m_nCurrentIndexU = 0;
		m_nCurrentIndexV = 0;
	}

	void CModelReaderNode_ExactGeometry1901_ControlPoints::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	
	void CModelReaderNode_ExactGeometry1901_ControlPoints::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		m_pWarnings->addException(CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_ExactGeometry1901_ControlPoints::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		

	}
	
	void CModelReaderNode_ExactGeometry1901_ControlPoints::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_NURBSSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_CONTROLPOINT) == 0)
			{
				PModelReaderNode_ExactGeometry1901_ControlPoint pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_ControlPoint>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				nfDouble dX, dY, dZ, dW;
				pXMLNode->retrievePoint(dX, dY, dZ, dW);

				nfUint32 nControlPointCountU = m_pNurbsSurface->getControlPointCountU();
				nfUint32 nControlPointCountV = m_pNurbsSurface->getControlPointCountV();

				if (m_nCurrentIndexU >= nControlPointCountU)
					throw CNMRException(NMR_ERROR_NURBSINVALIDCONTROLPOINTCOUNT);
				if (m_nCurrentIndexV >= nControlPointCountV)
					throw CNMRException(NMR_ERROR_NURBSINVALIDCONTROLPOINTCOUNT);

				m_pNurbsSurface->setControlPoint(m_nCurrentIndexU, m_nCurrentIndexV, dX, dY, dZ, dW);

				m_nCurrentIndexU++;
				if (m_nCurrentIndexU >= nControlPointCountU) {
					m_nCurrentIndexU = 0;
					m_nCurrentIndexV++;
				}
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue); 
		}
	}
}
