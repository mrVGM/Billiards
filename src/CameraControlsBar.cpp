#include "stdafx.h"

#include "CameraControlsBar.h"
#include "Physics.h"

#include "gtc\constants.hpp"

#include <sstream>

BEGIN_MESSAGE_MAP(CameraControlsBar, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeCaption)
	ON_EN_CHANGE(IDC_ACCELERATION, OnChangeCaption)
	ON_EN_CHANGE(IDC_SPEED, OnChangeCaption)
	ON_WM_HSCROLL(OnHScroll)
	ON_BN_CLICKED(IDC_STARTBUTTON, OnStartButtonClick)
END_MESSAGE_MAP()

CameraControlsBar::CameraControlsBar() : changed(false)
{
	dist = sqrt(3 * 100);
	parallel = 75;
	meridian = 87;
}

void CameraControlsBar::OnOK() 
{
	if (changed)
	{
		updateCamPosition();
	}
	changed = false;
}

void CameraControlsBar::OnCancel()
{}

BOOL CameraControlsBar::OnInitDialog()
{
	std::stringstream ss;
	ss << dist;

	CWnd * editBox = GetDlgItem(IDC_EDIT1);
	editBox->SetWindowTextA(ss.str().c_str());

	CSliderCtrl * slider1 = (CSliderCtrl *)(GetDlgItem(IDC_SLIDER1));
	CSliderCtrl * slider2 = (CSliderCtrl *)(GetDlgItem(IDC_SLIDER2));

	slider1->SetRangeMin(1);
	slider1->SetRangeMax(99);

	slider2->SetRangeMin(0);
	slider2->SetRangeMax(100);

	slider1->SetPos(parallel);
	slider2->SetPos(meridian);

	return 0;
}

void CameraControlsBar::OnChangeCaption()
{
	CWnd * editBox = GetDlgItem(IDC_EDIT1);
	
	CString cStr;
	editBox->GetWindowTextA(cStr);
	std::stringstream ss;
	ss << cStr.GetString();
	ss >> dist;
	changed = true;
}

void CameraControlsBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl * slider = (CSliderCtrl *)pScrollBar;
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
		parallel = slider->GetPos();
	else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER2)
		meridian = slider->GetPos();

	updateCamPosition();
}

void CameraControlsBar::OnStartButtonClick()
{
	Physics::getEngine().balls[0].direction = glm::normalize(glm::vec3(0, 1, 0));
	Physics::getEngine().balls[0].speed = 31.0f;
	Physics::getEngine().balls[0].stopped = false;

	Physics::getEngine().balls[1].direction = glm::normalize(glm::vec3(10, 30, 0));
	Physics::getEngine().balls[1].speed = 0.0f;
	Physics::getEngine().balls[1].stopped = false;
}

void CameraControlsBar::updateCamPosition()
{
	float par = glm::pi<float>() * parallel / 100 - glm::pi<float>() / 2.0f;
	float mer = 2.0f * glm::pi<float>() * meridian / 100;

	glm::vec3 pos;
	pos.z = sin(par);
	pos.x = cos(mer) * cos(par);
	pos.y = sin(mer) * cos(par);

	pos *= dist;

	App::getApp().mainWindow->renderWindow.renderer.setCamPosition(pos);
}