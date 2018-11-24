#include "editor_decoration.h"
#include "decorated_editor.h"

IEditorDecoration::IEditorDecoration(CDecoratedEditor* pParent)
	: QWidget((QWidget*)pParent)
	, m_pEditor(pParent)
{}


IEditorDecoration::~IEditorDecoration()
{}

CDecoratedEditor* IEditorDecoration::GetEditor() const
{
	return m_pEditor;
}

void IEditorDecoration::SetEditor(CDecoratedEditor* pEditor)
{
	m_pEditor = pEditor;
}
