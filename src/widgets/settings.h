#pragma once

#include <QDialog>
#include <QString>
#include "widgets_global.h"

class QSpinBox;

class WIDGETS_EXPORT CSettings : public QDialog
{

	Q_OBJECT

public:
	struct WIDGETS_EXPORT CSettingsSet
	{
		int tabWidth;
		QColor background;
		QColor alternateBackground;
		QColor foreground;
		QColor alternateForeground;
		struct
		{
			QColor normal;
			QColor hover;
			QColor pressed;
			QColor disabled;
			struct
			{
				QColor normal;
				QColor hover;
				QColor pressed;
				QColor disabled;
			} text_icon;
		} button;

		struct
		{
			QFont font;
			QColor color;
		} text;

		struct
		{
			QColor background;

			struct {
				QColor background;
				struct {
					QColor background;
					QColor bordercolor;
					int borderwidth;
				} breakpoint;
				struct {
					QColor background;
					QColor bordercolor;
					int borderwidth;
				} runingpoint;
			} breakpointarea;

			struct {
				QColor background;
				QColor foreground;
			} numberarea;

			struct {
				QFont font;
				QColor color;
			} text;

			struct
			{
				QColor background;
				QColor foreground;
				QColor border;
				int borderwidth;
			} currentline;
		} editor;

		struct
		{
			QColor background;
			QColor backgroundfocus;
			QColor foreground;
			QColor foregroundfocus;
			Qt::BrushStyle handlerstyle;
		} dockwidget;
		/*
		CSettingsSet()
			: tabWidth(4)
			, background(32, 32, 32)
			, alternateBackground(32, 128, 32)
			, foreground(216, 216, 216)
			, button
		{
			QColor(32,32,32),
			QColor(32,128,32),
			QColor(64,64,64),
			QColor(32,32,32),
			{
				QColor(216,216,216),
				QColor(216,216,216),
				QColor(216,216,216),
				QColor(216,216,216)
			}
		}
			, text
		{
			QFont("Ubuntu Mono", 12),
			QColor(216,216,216)
		}
		{}
		*/
		CSettingsSet()
			: tabWidth(4)
			, background(216,216,216)
			, alternateBackground(16, 128, 192)
			, foreground(0,0,0)
			, alternateForeground(255,255,255)
			, button
		{
			QColor(16,128,192),
			QColor(216,216,216),
			QColor(16,92,128),
			QColor(98,98,98),
			{
				QColor(255,255,255),
				QColor(0,0,0),
				QColor(255,255,255),
				QColor(64,64,64)
			}
		}
			, text
		{
			QFont("Ubuntu Mono", 12),
			QColor(0,0,0)
		}
			, editor
		{
			QColor(255,255,255),
			{
				QColor(192,192,192),
				{
					QColor(168,0,0),
					QColor(0,0,0),
					2
				},
				{
					QColor(0,255,0),
					QColor(0,168,0),
					2
				}
			},
			{
				QColor(216,216,216),
				QColor(16,32,64)
			},
			{
				QFont("Ubuntu Mono", 12),
				QColor(0,0,0)
			},
			{
				QColor(240,240,240),
				QColor(0,0,0),
				QColor(192,192,192),
				2
			}
		}
			, dockwidget
		{
			QColor(240, 240, 240),
			QColor(16, 128, 192),
			QColor(0,0,0),
			QColor(255,255,255),
			Qt::BrushStyle::Dense6Pattern
		}
		{}
	};

public:
	CSettings(QWidget* pParent = nullptr, QString const& strTitle = "", CSettingsSet const& sInitial = CSettingsSet());

	CSettingsSet GetSettings() const;
	CSettingsSet CollectSettings() const;

private:
	void SetupUI();
	void closeEvent(QCloseEvent* pEvent);

	inline bool IsUnsaved() { return m_bUnsaved; }

private slots:
	void OnOK();
	void OnApply();
	void OnCancel();
	void OnChanged();

signals:
	void SigChanged();

public:
	static CSettingsSet* GetGlobalDefaultSettings();
	static void SetGlobalDefaultSettings(CSettingsSet* pSettings);

private:
	static CSettingsSet* s_sGlobalSettings;
	CSettingsSet m_sSavedSettings;
	QSpinBox* fontSizes;

	bool m_bUnsaved;
};