/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

#include "../StandardPluginsConfig.h"
#include <MCore/Source/MemoryCategoriesCore.h>
#include <MysticQt/Source/DoubleSpinbox.h>
#include <MysticQt/Source/IntSpinbox.h>
#include <EMotionFX/CommandSystem/Source/SelectionCommands.h>
#include <EMotionFX/CommandSystem/Source/ActorCommands.h>
#include <EMotionFX/CommandSystem/Source/CommandManager.h>
#include "../../../../EMStudioSDK/Source/NodeSelectionWindow.h"

#include <QWidget>
#include <QTreeWidget>
#include <QMimeData>
#include <QUrl>
#include <QDropEvent>

QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QFileDialog)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QTableWidgetItem)
QT_FORWARD_DECLARE_CLASS(QShortcut)


namespace EMStudio
{
    class AttachmentsWindow
        : public QWidget
    {
        Q_OBJECT
        MCORE_MEMORYOBJECTCATEGORY(AttachmentsWindow, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS);

    public:
        AttachmentsWindow(QWidget* parent, bool deformable = false);
        ~AttachmentsWindow();

        void Init();
        void ReInit();

        void UpdateInterface();

        void AddAttachment(const AZStd::string& filename);
        void AddAttachments(const AZStd::vector<AZStd::string>& filenames);

        void OnAttachmentSelected();
        bool GetIsWaitingForAttachment() const      { return mWaitingForAttachment; }

        EMotionFX::ActorInstance* GetSelectedAttachment();
        MCore::String GetSelectedNodeName();
    protected:
        void dropEvent(QDropEvent* event) override;
        void dragEnterEvent(QDragEnterEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    private slots:
        void OnNodeChanged();
        void OnSelectionChanged();
        void OnAddAttachmentButtonClicked();
        void OnAddDeformableAttachmentButtonClicked();
        void OnOpenAttachmentButtonClicked();
        void OnOpenDeformableAttachmentButtonClicked();
        void OnRemoveButtonClicked();
        void OnClearButtonClicked();
        void OnSelectNodeButtonClicked();
        void OnDroppedAttachmentsActors();
        void OnDroppedDeformableActors();
        void OnVisibilityChanged(int visibility);
        void OnFastUpdatesChanged(int fastUpdates);
        void OnAttachmentNodesSelected(MCore::Array<SelectionItem> selection);
        void OnCancelAttachmentNodeSelection();
        void OnEscapeButtonPressed();
        void OnUpdateButtonsEnabled();

    private:
        void RemoveTableItems(const QList<QTableWidgetItem*>& items);
        int GetIDFromTableRow(int row);
        MCore::String GetNodeNameFromTableRow(int row);
        int GetRowContainingWidget(const QWidget* widget);

        bool                                    mWaitingForAttachment;
        bool                                    mIsDeformableAttachment;

        QVBoxLayout*                            mWaitingForAttachmentLayout;
        QVBoxLayout*                            mNoSelectionLayout;
        QVBoxLayout*                            mMainLayout;
        QVBoxLayout*                            mAttachmentsLayout;

        QWidget*                                mAttachmentsWidget;
        QWidget*                                mWaitingForAttachmentWidget;
        QWidget*                                mNoSelectionWidget;

        QShortcut*                              mEscapeShortcut;

        QTableWidget*                           mTableWidget;
        EMotionFX::ActorInstance*               mActorInstance;
        AZStd::vector<AZStd::string>            mAttachments;
        AZStd::string                           mNodeBeforeSelectionWindow;

        QPushButton*                            mAddAttachmentButton;
        QPushButton*                            mAddDeformableAttachmentButton;
        QPushButton*                            mOpenAttachmentButton;
        QPushButton*                            mOpenDeformableAttachmentButton;
        QPushButton*                            mRemoveButton;
        QPushButton*                            mClearButton;
        QPushButton*                            mCancelSelectionButton;

        NodeSelectionWindow*                    mNodeSelectionWindow;

        AZStd::vector<AZStd::string>            mDropFileNames;
        MCore::String                           mTempString;
    };

} // namespace EMStudio