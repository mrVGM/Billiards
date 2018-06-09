#pragma once

#include <afxwin.h>

class CameraControlsBar : public CDialog
{
	bool changed;
	float dist;
	int parallel;
	int meridian;
public:
	CameraControlsBar();
	void OnOK() override;
	void OnCancel() override;
	BOOL OnInitDialog() override;
	
	void OnChangeCaption();
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void OnStartButtonClick();

	void updateCamPosition();

	DECLARE_MESSAGE_MAP()
};