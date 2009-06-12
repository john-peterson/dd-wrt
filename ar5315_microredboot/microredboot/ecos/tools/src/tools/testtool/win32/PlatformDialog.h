//####COPYRIGHTBEGIN####
//                                                                          
// ----------------------------------------------------------------------------
// Copyright (C) 1998, 1999, 2000 Red Hat, Inc.
//
// This program is part of the eCos host tools.
//
// This program is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation; either version 2 of the License, or (at your option) 
// any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
// more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// ----------------------------------------------------------------------------
//                                                                          
//####COPYRIGHTEND####
#if !defined(AFX_PlatformDialog_H__EEE60768_E2E3_11D3_A543_00A0C949ADAC__INCLUDED_)
#define AFX_PlatformDialog_H__EEE60768_E2E3_11D3_A543_00A0C949ADAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatformDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlatformDialog dialog
#include "testtoolres.h"
#include "eCosDialog.h"
#include "eCosTest.h"

class CPlatformDialog : public CeCosDialog
{
// Construction
public:
	CPlatformDialog(CWnd* pParent = NULL);   // standard constructor
  CString m_strCaption;
// Dialog Data
	//{{AFX_DATA(CPlatformDialog)
	enum { IDD = IDD_TT_PLATFORM };
	CString m_strPlatform;
	CString m_strPrefix;
	CString m_strGDB;
	CString	m_strInferior;
	CString	m_strPrompt;
	BOOL	m_bServerSideGdb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlatformDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPlatformDialog)
  	virtual BOOL OnInitDialog();
	afx_msg void OnChangeNewPlatformPrefix();
	afx_msg void OnChangeNewPlatform();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PlatformDialog_H__EEE60768_E2E3_11D3_A543_00A0C949ADAC__INCLUDED_)
