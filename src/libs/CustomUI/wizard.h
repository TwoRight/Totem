/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef WIZARD_H
#define WIZARD_H

#include "customui_global.h"

#include <QWizard>
#include <QPixmap>
QT_BEGIN_NAMESPACE
class QLabel;
class QVBoxLayout;
QT_END_NAMESPACE
namespace CustomUI {

class Wizard;
class WizardProgress;
class WizardPrivate;

class CUSTOMUI_EXPORT Wizard : public QWizard
{
    Q_OBJECT
    Q_PROPERTY(bool automaticProgressCreationEnabled READ isAutomaticProgressCreationEnabled WRITE setAutomaticProgressCreationEnabled)

public:
    explicit Wizard(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~Wizard();

    bool isAutomaticProgressCreationEnabled() const;
    void setAutomaticProgressCreationEnabled(bool enabled);

    void setStartId(int pageId);

    WizardProgress *wizardProgress() const;
    virtual bool validateCurrentPage();

signals:
    void nextClicked(); /* workaround for QWizard behavior where page->initialize is
                         * called before currentIdChanged */

private slots:
    void _q_currentPageChanged(int pageId);
    void _q_pageAdded(int pageId);
    void _q_pageRemoved(int pageId);

private:
    Q_DECLARE_PRIVATE(Wizard)

    class WizardPrivate *d_ptr;
};

class WizardProgressItem;
class WizardProgressPrivate;

class CUSTOMUI_EXPORT WizardProgress : public QObject
{
    Q_OBJECT

public:
    WizardProgress(QObject *parent = 0);
    ~WizardProgress();

    WizardProgressItem *addItem(const QString &title);
    void removeItem(WizardProgressItem *item);

    void removePage(int pageId);

    QList<int> pages(WizardProgressItem *item) const;
    WizardProgressItem *item(int pageId) const;

    WizardProgressItem *currentItem() const;

    QList<WizardProgressItem *> items() const;

    WizardProgressItem *startItem() const;

    QList<WizardProgressItem *> visitedItems() const;
    QList<WizardProgressItem *> directlyReachableItems() const;
    bool isFinalItemDirectlyReachable() const; // return  availableItems().last()->isFinalItem();

Q_SIGNALS:
    void currentItemChanged(WizardProgressItem *item);

    void itemChanged(WizardProgressItem *item); // contents of the item: title or icon
    void itemAdded(WizardProgressItem *item);
    void itemRemoved(WizardProgressItem *item);
    void nextItemsChanged(WizardProgressItem *item, const QList<WizardProgressItem *> &items);
    void nextShownItemChanged(WizardProgressItem *item, WizardProgressItem *nextShownItem);
    void startItemChanged(WizardProgressItem *item);

private:
    void setCurrentPage(int pageId);
    void setStartPage(int pageId);

private:
    friend class Wizard;
    friend class WizardProgressItem;

    Q_DECLARE_PRIVATE(WizardProgress)

    class WizardProgressPrivate *d_ptr;
};

class WizardProgressItemPrivate;

class CUSTOMUI_EXPORT WizardProgressItem // managed by WizardProgress
{

public:
    void addPage(int pageId);
    QList<int> pages() const;
    void setNextItems(const QList<WizardProgressItem *> &items);
    QList<WizardProgressItem *> nextItems() const;
    void setNextShownItem(WizardProgressItem *item);
    WizardProgressItem *nextShownItem() const;
    bool isFinalItem() const; // return nextItems().isEmpty();

    void setTitle(const QString &title);
    QString title() const;
    void setTitleWordWrap(bool wrap);
    bool titleWordWrap() const;

protected:
    WizardProgressItem(WizardProgress *progress, const QString &title);
    virtual ~WizardProgressItem();

private:
    friend class WizardProgress;

    Q_DECLARE_PRIVATE(WizardProgressItem)

    class WizardProgressItemPrivate *d_ptr;
};

class ProgressItemWidget : public QWidget
{
	Q_OBJECT
public:
	ProgressItemWidget(const QPixmap &indicatorPixmap, const QString &title, QWidget *parent = 0);
	void setIndicatorVisible(bool visible);
	void setTitle(const QString &title);
	void setWordWrap(bool wrap);

private:
	bool m_indicatorVisible;
	QPixmap m_indicatorPixmap;
	QLabel *m_indicatorLabel;
	QLabel *m_titleLabel;
};

class LinearProgressWidget : public QWidget
{
    Q_OBJECT
public:
    LinearProgressWidget(WizardProgress *progress, QWidget *parent = 0);

private slots:
    void slotItemAdded(WizardProgressItem *item);
    void slotItemRemoved(WizardProgressItem *item);
    void slotItemChanged(WizardProgressItem *item);
    void slotNextItemsChanged(WizardProgressItem *item, const QList<WizardProgressItem *> &nextItems);
    void slotNextShownItemChanged(WizardProgressItem *item, WizardProgressItem *nextItem);
    void slotStartItemChanged(WizardProgressItem *item);
    void slotCurrentItemChanged(WizardProgressItem *item);

private:
    void recreateLayout();
    void updateProgress();
    void disableUpdates();
    void enableUpdates();

    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_itemWidgetLayout;
    WizardProgress *m_wizardProgress;
    QMap<WizardProgressItem *, ProgressItemWidget *> m_itemToItemWidget;
    QMap<ProgressItemWidget *, WizardProgressItem *> m_itemWidgetToItem;
    QList<WizardProgressItem *> m_visibleItems;
    ProgressItemWidget *m_dotsItemWidget;
    int m_disableUpdatesCount;
    QPixmap m_indicatorPixmap;
};
} // namespace Utils

#endif // WIZARD_H
