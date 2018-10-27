#pragma once

#include "plain_editor_global.h"
#include <QPlainTextEdit>
#include <QTextBlock>

class PLAIN_EDITOR_EXPORT CPublicPlainTextEdit : public QPlainTextEdit
{
//public:
//	CPublicPlainTextEdit(QWidget *parent = nullptr)
//		: QPlainTextEdit(parent) {}
//	CPublicPlainTextEdit(const QString &text, QWidget *parent = nullptr)
//		: QPlainTextEdit(text, parent) {}
//	~CPublicPlainTextEdit() {}
//
//public: // changing scope of protected functions
//	QRectF blockBoundingGeometry(const QTextBlock &block) const { return QPlainTextEdit::blockBoundingGeometry(block); }
//	QRectF blockBoundingRect(const QTextBlock &block) const { return QPlainTextEdit::blockBoundingRect(block); }
//	virtual bool canInsertFromMimeData(const QMimeData *source) const { return QPlainTextEdit::canInsertFromMimeData(source); }
//	QPointF	contentOffset() const { return QPlainTextEdit::contentOffset(); }
//	virtual QMimeData *	createMimeDataFromSelection() const { return QPlainTextEdit::createMimeDataFromSelection(); }
//	QTextBlock firstVisibleBlock() const { return QPlainTextEdit::firstVisibleBlock(); }
//	QAbstractTextDocumentLayout::PaintContext	getPaintContext() const { return QPlainTextEdit::getPaintContext(); }
//	virtual void insertFromMimeData(const QMimeData *source) { insertFromMimeData(source); }
	friend class CBreakpointDecoration;
	friend class CLineNumberDecoration;
};

