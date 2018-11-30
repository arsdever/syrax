#include "editormgr.h"

#include <tabwidget.h>
#include <manipulators.h>

#include <QPlainTextEdit>

CEditorManager* CEditorManager::s_pGlobalInstance = nullptr;

CEditorManager::CEditorManager(QObject* pParent)
	: QObject(pParent)
	, m_pTabWidget(new CTabWidget())
	, m_nCurrentEditor(-1)
	, m_arrUntitledIndexes(256, nullptr)
	, m_itLastUntitled(m_arrUntitledIndexes.begin())
{
	m_pTabWidget->setTabsClosable(true);
	connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(OnClose(int)));
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(OnCurrentChanged(int)));
}

CEditorManager::~CEditorManager()
{
}

CEditor* CEditorManager::GetCurrentEditor() const
{
	if (m_nCurrentEditor == -1)
		return nullptr;

	return m_lstEditors.at(m_nCurrentEditor);
}

CEditor* CEditorManager::GetEditor(QPlainTextEdit* pEditor) const
{
	int index = GetTabIndex(pEditor);

	if (index > -1)
		return m_lstEditors.at(index);

	return nullptr;
}

qint32 CEditorManager::GetCurrentEditorIndex() const
{
	return m_nCurrentEditor;
}

CEditor* CEditorManager::GetEditorAt(int nIndex) const
{
	return m_lstEditors.at(nIndex);
}

void CEditorManager::SetCurrentEditor(int nIndex)
{
	if (nIndex >= m_lstEditors.size())
		return;

	m_nCurrentEditor = nIndex;
	m_pTabWidget->setCurrentIndex(nIndex);

	emit CurrentEditorChanged(nIndex);
}

void CEditorManager::SetCurrentEditor(CEditor* pEditor)
{
	SetCurrentEditor(GetTabIndex(pEditor));
}

void CEditorManager::SetCurrentEditor(QPlainTextEdit* pEditor)
{
	SetCurrentEditor(GetTabIndex(pEditor));
}

void CEditorManager::SetTabWidget(QTabWidget* pTabWidget)
{
	m_pTabWidget = pTabWidget;
}

QTabWidget* CEditorManager::GetTabWidget() const
{
	return m_pTabWidget;
}

int CEditorManager::GetTabIndex(CEditor* pEditor) const
{
	return m_lstEditors.indexOf(pEditor);
}

int CEditorManager::GetTabIndex(QPlainTextEdit* pEditor) const
{
	auto it = m_lstEditors.cbegin();
	int i = 0;
	for (; it != m_lstEditors.cend(); ++i, ++it)
	{
		if ((*it)->GetCoreWidget() == pEditor)
			return i;
	}
	return -1;
}

void CEditorManager::XFileManipulator::Open(QStringList const& strPaths)
{
	for (QString path : strPaths)
	{
		bool found = false;
		CEditor* editor = nullptr;
		for (CEditor* pEditor : m_pThis->m_lstEditors)
		{
			if (pEditor == nullptr)
				continue;

			if (pEditor->IsValid() && pEditor->GetFilePath() == path)
			{
				found = true;
				editor = pEditor;
				break;
			}
		}

		if (!found)
		{
			editor = new CEditor(path);
			if (!editor->IsValid())
			{
				delete editor;
				continue;
			}

			m_pThis->m_lstEditors.push_back(editor);

			if (m_pThis->m_pTabWidget == nullptr)
				return;

			int newTabIndex = m_pThis->m_pTabWidget->addTab(editor->GetCoreWidget(), editor->GetFileName());
			m_pThis->m_pTabWidget->setTabToolTip(newTabIndex, editor->GetFilePath());
		}

		m_pThis->SetCurrentEditor(editor);
	}
	//m_pThis->SetCurrentEditor();
}

void CEditorManager::XFileManipulator::SaveAll()
{
	for (CEditor* editor : m_pThis->m_lstEditors)
	{
		if (!editor->Save())
			return;
	}
}

void CEditorManager::XFileManipulator::Close(qint32 nIndex)
{
	if (nIndex < 0)
	{
		if (m_pThis->m_nCurrentEditor < 0)
			return;

		nIndex = m_pThis->m_nCurrentEditor;
	}

	CEditor* pClosing = m_pThis->GetEditorAt(nIndex);

	qint32 untitledIndex = m_pThis->m_arrUntitledIndexes.indexOf(pClosing);
	if (untitledIndex != -1)
		m_pThis->m_arrUntitledIndexes[untitledIndex] = nullptr;

	m_pThis->m_pTabWidget->removeTab(nIndex);
	delete pClosing;
	m_pThis->m_lstEditors.removeAt(nIndex);
}

bool CEditorManager::XFileManipulator::AskForClose(qint32 nIndex, IFileManipulator::EClosingType eType)
{
	switch (eType)
	{
	case IFileManipulator::Single:
	{
		CEditor* pClosing = nullptr;
		if (nIndex == -1)
			pClosing = m_pThis->GetCurrentEditor();
		else
			pClosing = m_pThis->GetEditorAt(nIndex);

		if (pClosing != nullptr && !pClosing->AskForSave())
			return false;
	
		Close(nIndex);
		return true;
	}		
	case IFileManipulator::Right:
		for (int i = m_pThis->m_lstEditors.size() - 1; i > nIndex; --i)
		{
			if (!AskForClose(i, IFileManipulator::Single))
				return false;
		}
		return true;
	case IFileManipulator::Left:
		for (int i = nIndex - 1; i >= 0; --i)
		{
			if (!AskForClose(i, IFileManipulator::Single))
				return false;
		}
		return true;
	case IFileManipulator::All:
		for (int i = m_pThis->m_lstEditors.size() - 1; i >= 0; --i)
		{
			if (!AskForClose(i, IFileManipulator::Single))
				return false;
		}
		return true;
	}

	return true;
}

void CEditorManager::XFileManipulator::Save(QString const&)
{
	m_pThis->GetCurrentEditor()->Save();
}

void CEditorManager::XFileManipulator::SaveAs()
{

}

void CEditorManager::XFileManipulator::New()
{
	CEditor* pNew = new CEditor();
	m_pThis->m_lstEditors.push_back(pNew);
	qint32 index = 0;

	for (void* ptr : m_pThis->m_arrUntitledIndexes)
		if (ptr != nullptr)
			++index;
		else
			break;

	pNew->SetTitle(QString("Untitled tab %1").arg(QString::number(index + 1)));
	m_pThis->m_arrUntitledIndexes[index] = pNew;

	if (m_pThis->m_pTabWidget == nullptr)
		return;

	int newTabIndex = m_pThis->m_pTabWidget->addTab(pNew->GetCoreWidget(), pNew->GetTitle());
	m_pThis->m_pTabWidget->setTabToolTip(newTabIndex, pNew->GetFilePath());
	connect(pNew, SIGNAL(TitleUpdated()), m_pThis, SLOT(OnTitleUpdated()));
}

CEditorManager* CEditorManager::GlobalInstance()
{
	return s_pGlobalInstance == nullptr ? s_pGlobalInstance = new CEditorManager(GetCore()) : s_pGlobalInstance;
}

void CEditorManager::OnClose(int nIndex)
{
	CallFunctionAndReturn<IFileManipulator>(IFileManipulator::AskForCloseFunctor(nIndex));
}

void CEditorManager::OnCurrentChanged(int nIndex)
{
	m_nCurrentEditor = nIndex;
}

void CEditorManager::OnTitleUpdated()
{
	qint32 idx = m_arrUntitledIndexes.indexOf(sender());
	if (idx != -1)
		m_arrUntitledIndexes[idx] = nullptr;
}

extern "C" EDITOR_EXPORT void LoadPlugin()
{
	CEditorManager::GlobalInstance();
}