#pragma once

#include <QPlainTextEdit>

class CPublicPlainTextEdit : public QPlainTextEdit
{
public:
	CPublicPlainTextEdit();
	~CPublicPlainTextEdit();

	friend class IEditorDecoration;
	friend class CLineNumberDecoration;
};

