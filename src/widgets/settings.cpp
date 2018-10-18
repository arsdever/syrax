#include "settings.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QSpinBox>

CSettings::CSettingsSet* CSettings::s_sGlobalSettings = new CSettings::CSettingsSet();

CSettings::CSettings(QWidget* pParent, QString const& strTitle, CSettingsSet const& sInitial)
	: QDialog(pParent)
	, m_sSavedSettings(sInitial)
	, m_bUnsaved(false)
{
	setWindowTitle(strTitle);
	SetupUI();
}

void CSettings::SetupUI()
{
	QPushButton* pbOK = new QPushButton("OK");
	QPushButton* pbApply = new QPushButton("Apply");
	QPushButton* pbCancel = new QPushButton("Cancel");

	connect(pbOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(pbApply, SIGNAL(clicked()), this, SLOT(OnApply()));
	connect(pbCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* resultButtons = new QHBoxLayout();
	QGridLayout* toolsLayout = new QGridLayout();

	resultButtons->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Fixed));
	resultButtons->addWidget(pbOK);
	resultButtons->addWidget(pbApply);
	resultButtons->addWidget(pbCancel);

	fontSizes = new QSpinBox();
	fontSizes->setMinimum(6);
	fontSizes->setMaximum(48);
	fontSizes->setSingleStep(1);
	fontSizes->setValue(m_sSavedSettings.text.font.pointSize());
	connect(fontSizes, SIGNAL(valueChanged()), this, SLOT(OnChanged()));

	toolsLayout->addWidget(fontSizes);

	mainLayout->addLayout(toolsLayout);
	mainLayout->addLayout(resultButtons);

	setLayout(mainLayout);
}

void CSettings::OnOK()
{
	m_sSavedSettings = CollectSettings();
	close();
}

void CSettings::OnApply()
{
	m_sSavedSettings = CollectSettings();
}

void CSettings::OnCancel()
{
	close();
}

void CSettings::closeEvent(QCloseEvent* pEvent)
{
	if (IsUnsaved())
	{
		QMessageBox::StandardButtons result = QMessageBox::question(this, "Cancel changes", "You have unsaved changes. Do you want to discard them?", QMessageBox::Yes, QMessageBox::No);
		if (result != QMessageBox::No)
			QDialog::closeEvent(pEvent);
	}
}

CSettings::CSettingsSet CSettings::CollectSettings() const
{
	CSettingsSet settings;
	settings.text.font.setPointSize(fontSizes->value());

	return settings;
}

CSettings::CSettingsSet CSettings::GetSettings() const
{
	return m_sSavedSettings;
}

void CSettings::OnChanged()
{
	m_bUnsaved = true;
	emit SigChanged();
}

CSettings::CSettingsSet* CSettings::GetGlobalDefaultSettings()
{
	return s_sGlobalSettings;
}

void CSettings::SetGlobalDefaultSettings(CSettings::CSettingsSet* pSettings)
{
	delete s_sGlobalSettings;
	s_sGlobalSettings = pSettings;
}