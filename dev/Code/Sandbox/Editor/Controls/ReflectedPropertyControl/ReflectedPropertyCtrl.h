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

#ifndef CRYINCLUDE_EDITOR_UTILS_REFLECTEDPROPERTYCTRL_H
#define CRYINCLUDE_EDITOR_UTILS_REFLECTEDPROPERTYCTRL_H
#pragma once

#include <AzToolsFramework/UI/PropertyEditor/PropertyEditorAPI.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include "Include/EditorCoreAPI.h"
#include "ReflectedPropertyItem.h"
#include "ReflectedVar.h"
#include <QFrame>

class QLineEdit;
class QLabel;
class QVBoxLayout;
class PropertyCard;
class QScrollArea;

namespace AzToolsFramework {
    class ReflectedPropertyEditor;
    class PropertyRowWidget;
    class ComponentEditorHeader;
}

//ReflectedPropertyEditor-based implementation of the MFC CPropertyCtrl API
class ReflectedPropertyControl
    : public QWidget
    , public AzToolsFramework::IPropertyEditorNotify
{
    Q_OBJECT
public:
    //! For alternative undo.
    typedef Functor1<IVariable*> UndoCallback;

    explicit ReflectedPropertyControl(QWidget* parent = nullptr, Qt::WindowFlags windowFlags = Qt::WindowFlags());

    void Setup(bool showScrollbars = true, int labelWidth = 150);

    ReflectedPropertyItem* AddVarBlock(CVarBlock* varBlock, const char* szCategory = nullptr);

    void CreateItems(XmlNodeRef node);
    void CreateItems(XmlNodeRef node, CVarBlockPtr& varBlock, IVariable::OnSetCallback func, bool splitCamelCaseIntoWords = false);

    // Replace category item contents with the specified var block.
    virtual void ReplaceVarBlock(IVariable* categoryItem, CVarBlock* varBlock);

    //replace top-level var block.  (used to port ctrl->ReplaceVarBlock(ctrl->GetRootItem(), varBlock);
    virtual void ReplaceRootVarBlock(CVarBlock* newVarBlock);

    void RemoveAllItems();

    bool FindVariable(IVariable* categoryItem) const;

    //! When item change, this callback fired variable that changed.
    typedef Functor1<IVariable*> UpdateVarCallback;
    //! When item change, update object.
    typedef Functor1<IVariable*> UpdateObjectCallback;
    //! When selection changes, this callback fired variable that changed.
    typedef Functor1<IVariable*> SelChangeCallback;

    /** Set update callback to be used for this property window.
    */
    void SetUpdateCallback(const UpdateVarCallback& callback);
    void ClearUpdateCallback() { m_updateVarFunc = nullptr; }

    void SetUpdateObjectCallback(UpdateObjectCallback callback) { m_updateObjectFunc = callback; }
    void ClearUpdateObjectCallback() { m_updateObjectFunc = nullptr; }

    /** Set selchange callback to be used for this property window.
    */
    void SetSelChangeCallback(SelChangeCallback callback) { m_selChangeFunc = callback; }

    //set a key that can be used to save/restore expanded state.
    void SetSavedStateKey(AZ::u32 key);

    void ExpandAll();
    void CollapseAll();

    virtual void Expand(ReflectedPropertyItem* item, bool expand);
    void ExpandAllChildren(ReflectedPropertyItem* item, bool recursive);

    //IPropertyEditorNotify Interface
    void BeforePropertyModified(AzToolsFramework::InstanceDataNode* pNode) override {}
    void AfterPropertyModified(AzToolsFramework::InstanceDataNode* pNode) override;
    void SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* pNode) override {}
    void SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* pNode) override {}
    void SealUndoStack() override {}
    void RequestPropertyContextMenu(AzToolsFramework::InstanceDataNode*, const QPoint&) override;
    void PropertySelectionChanged(AzToolsFramework::InstanceDataNode *pNode, bool selected) override;

    void SetStoreUndoByItems(bool bStoreUndoByItems) { m_bStoreUndoByItems = bStoreUndoByItems; }
    bool IsStoreUndoByItems() const { return m_bStoreUndoByItems; }

    void ClearSelection();
    ReflectedPropertyItem* GetSelectedItem();
    virtual void SelectItem(ReflectedPropertyItem* item);
    void SetDisplayOnlyModified(bool displayOnlyModified);

    QVector<ReflectedPropertyItem*> GetSelectedItems();

    /** Set alternative undo callback.
    */
    void SetUndoCallback(UndoCallback& callback);
    void ClearUndoCallback();

    /** Enable of disable calling update callback when some values change.
    */
    void EnableUpdateCallback(bool bEnable);
    void SetDeferredUpdateCallback(bool deferred);

    // Control is grayed, but is not readonly.
    void SetGrayed(bool grayed);

    // Sets control to be read only, User cannot modify content of properties.
    void SetReadOnly(bool readonly);

    void SetMultiSelect(bool multiSelect);

    void SetSplitter(int width);

    void EnableNotifyWithoutValueChange(bool bFlag);

    void CopyItem(XmlNodeRef rootNode, ReflectedPropertyItem* pItem, bool bRecursively);

    // set to false if you don't want to receive callbacks when the item is not modified (when items are expanded etc)
    void SetCallbackOnNonModified(bool bEnable) { m_bSendCallbackOnNonModified = bEnable; }

    void ReloadValues();

    //whether to group child properties alphabetically under expanding elements
    void SetGroupProperties(bool group);

    //whether to sort child properties alphabetically
    void SetSortProperties(bool sort);

    //whether to show line edit for filtering properties
    void SetShowFilterWidget(bool showFilter);

    // It doesn't add any property item, but instead it updates all property items with
    // a new variable block (sets display value, flags and user data).
    virtual void UpdateVarBlock(CVarBlock* pVarBlock);

    //! Find item that reference specified property.
    ReflectedPropertyItem* FindItemByVar(IVariable* pVar);

    ReflectedPropertyItem* GetRootItem();

    int GetContentHeight() const;
    int GetVisibleHeight() const {return GetContentHeight();}

    void SetRootName(const QString& rootName);

    struct SCustomPopupItem
    {
        typedef Functor0 Callback;

        QString m_text;
        Callback m_callback;

        SCustomPopupItem(const QString& text, const Functor0& callback)
            : m_text(text)
            , m_callback(callback) {}
    };

    struct SCustomPopupMenu
    {
        typedef Functor1<int> Callback;

        QString m_text;
        Callback m_callback;
        QStringList m_subMenuText;

        SCustomPopupMenu(const QString& text, const Callback& callback, const QStringList& subMenuText)
            : m_text(text)
            , m_callback(callback)
            , m_subMenuText(subMenuText) {}
    };

    void AddCustomPopupMenuPopup(const QString& text, const Functor1<int>& handler, const QStringList& items);
    void RemoveCustomPopupMenuPopup(const QString& text);

    void AddCustomPopupMenuItem(const QString& text, const SCustomPopupItem::Callback handler);
    void RemoveCustomPopupMenuItem(const QString& text);

    AzToolsFramework::PropertyRowWidget* FindPropertyRowWidget(ReflectedPropertyItem* item);

    QSize sizeHint() const override;
    AzToolsFramework::ReflectedPropertyEditor* GetEditor() { return m_editor; }

public slots:
    //invalidates attributes and values
    void InvalidateCtrl(bool queued = true);
    void RebuildCtrl(bool queued = true);

    void SetTitle(const QString &title);

    void OnCopy(bool bRecursively);
    void OnCopyAll();
    void OnPaste();

protected:
    friend class ReflectedPropertyItem;

    void OnItemChange(ReflectedPropertyItem* item);
    CReflectedVar* GetReflectedVarFromCallbackInstance(AzToolsFramework::InstanceDataNode* pNode);
    void RecreateAllItems();

    // only shows items containing the string in their name. All items shown if string is empty.
    void RestrictToItemsContaining(const QString& searchName);

    bool CallUndoFunc(ReflectedPropertyItem* item);

    virtual void UpdateVarBlock(ReflectedPropertyItem* pPropertyItem, IVariableContainer* pSourceContainer, IVariableContainer* pTargetContainer);

    void ClearVarBlock();

private slots:
    void DoUpdateCallback(IVariable *var);
    void DoUpdateObjectCallback(IVariable *var);

private:
    AzToolsFramework::ReflectedPropertyEditor* m_editor;
    QLineEdit* m_filterLineEdit;
    QWidget* m_filterWidget;
    QLabel* m_titleLabel;

    TSmartPtr<CVarBlock> m_pVarBlock;
    TSmartPtr<ReflectedPropertyItem> m_root;
    AZStd::unique_ptr<CPropertyContainer> m_rootContainer;

    AZ::SerializeContext* m_serializeContext;

    bool m_bEnableCallback;
    QString m_filterString;
    UpdateVarCallback m_updateVarFunc;
    UpdateObjectCallback m_updateObjectFunc;
    SelChangeCallback m_selChangeFunc;
    UndoCallback m_undoFunc;

    bool m_bStoreUndoByItems;
    bool m_bForceModified;

    bool m_groupProperties;
    bool m_sortProperties;
    bool m_bSendCallbackOnNonModified;
    bool m_initialized;

    //custom popup menu
    std::vector<SCustomPopupItem> m_customPopupMenuItems;
    std::vector<SCustomPopupMenu> m_customPopupMenuPopups;
    template<typename T>
    void RemoveCustomPopup(const QString& text, T& customPopup);
};


class TwoColumnPropertyControl
    : public QWidget
{
    Q_OBJECT
public:
    TwoColumnPropertyControl(QWidget* parent = nullptr);
    void Setup(bool showScrollbars = true, int labelWidth = 150);

    void AddVarBlock(CVarBlock* varBlock, const char* szCategory = nullptr);

    // Replace category item contents with the specified var block.
    virtual void ReplaceVarBlock(IVariable* categoryItem, CVarBlock* varBlock);

    void RemoveAllItems();

    bool FindVariable(IVariable* categoryItem) const;

    void InvalidateCtrl();
    void RebuildCtrl();

    void SetStoreUndoByItems(bool bStoreUndoByItems);

    /** Set alternative undo callback.
    */
    void SetUndoCallback(ReflectedPropertyControl::UndoCallback callback);
    void ClearUndoCallback();

    /** Enable of disable calling update callback when some values change.
    */
    void EnableUpdateCallback(bool bEnable);
    void SetUpdateCallback(ReflectedPropertyControl::UpdateVarCallback callback);

    // Control is grayed, but is not readonly.
    void SetGrayed(bool grayed);

    //set a key that can be used to save/restore expanded state.
    void SetSavedStateKey(const QString& key);

    void ExpandAllChildren(ReflectedPropertyItem* item, bool recursive);
    void ExpandAllChildren(bool recursive);

    void ReloadItems();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void ToggleTwoColumnLayout();

    QVector<PropertyCard*> m_controlList;
    QVector<TSmartPtr<CVarBlock>> m_varBlockList;
    TSmartPtr<CVarBlock> m_pVarBlock;

    QWidget *m_leftContainer;
    QWidget *m_rightContainer;
    QScrollArea *m_leftScrollArea;
    QScrollArea *m_rightScrollArea;

    bool m_twoColumns;

    static const int minimumColumnWidth = 320;
    static const int minimumTwoColumnWidth = 660;
};


class PropertyCard
    : public QFrame
{
    Q_OBJECT
public:
    PropertyCard(QWidget* parent = nullptr);

    void AddVarBlock(CVarBlock *varBlock);

    ReflectedPropertyControl* GetControl();

    void SetExpanded(bool expanded);
    bool IsExpanded() const;

Q_SIGNALS:
    void OnExpansionContractionDone();

private:
    void OnExpanderChanged(bool expanded);

    AzToolsFramework::ComponentEditorHeader* m_header = nullptr;
    ReflectedPropertyControl* m_propertyEditor = nullptr;
};

#endif // CRYINCLUDE_EDITOR_UTILS_REFLECTEDPROPERTYCTRL_H
