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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWKEYPROPERTIESDLG_H
#define CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWKEYPROPERTIESDLG_H
#pragma once

#pragma once

#include "TrackViewSequence.h"
#include "TrackViewNode.h"
#include "Plugin.h"
#include "TrackViewDopeSheetBase.h"
#include "QtViewPane.h"

#include <QScopedPointer>
#include <QDockWidget>

namespace Ui {
    class CTrackViewTrackPropsDlg;
    class CTrackViewKeyPropertiesDlg;
}

class ReflectedPropertyControl;
class CTrackViewKeyPropertiesDlg;

//////////////////////////////////////////////////////////////////////////
class CTrackViewKeyUIControls
    : public QObject
    , public _i_reference_target_t
{
    Q_OBJECT
public:
    CTrackViewKeyUIControls() { m_pVarBlock = new CVarBlock; };

    void SetKeyPropertiesDlg(CTrackViewKeyPropertiesDlg* pDlg) { m_pKeyPropertiesDlg = pDlg; }

    // Return internal variable block.
    CVarBlock* GetVarBlock() const { return m_pVarBlock; }

    //////////////////////////////////////////////////////////////////////////
    // Callbacks that must be implemented in derived class
    //////////////////////////////////////////////////////////////////////////
    // Returns true if specified animation track type is supported by this UI.
    virtual bool SupportTrackType(const CAnimParamType& paramType, EAnimCurveType trackType, EAnimValue valueType) const = 0;

    // Called when UI variable changes.
    virtual void OnCreateVars() = 0;

    // Called when user changes selected keys.
    // Return true if control update UI values
    virtual bool OnKeySelectionChange(CTrackViewKeyBundle& keys) = 0;

    // Called when UI variable changes.
    virtual void OnUIChange(IVariable* pVar, CTrackViewKeyBundle& keys) = 0;

    // Get priority of key UI control, so that specializations can have precedence
    virtual unsigned int GetPriority() const = 0;

protected:
    //////////////////////////////////////////////////////////////////////////
    // Helper functions.
    //////////////////////////////////////////////////////////////////////////
    template <class T>
    void SyncValue(CSmartVariable<T>& var, T& value, bool bCopyToUI, IVariable* pSrcVar = NULL)
    {
        if (bCopyToUI)
        {
            var = value;
        }
        else
        {
            if (!pSrcVar || pSrcVar == var.GetVar())
            {
                value = var;
            }
        }
    }
    void AddVariable(CVariableBase& varArray, CVariableBase& var, const char* varName, unsigned char dataType = IVariable::DT_SIMPLE)
    {
        if (varName)
        {
            var.SetName(varName);
        }
        var.SetDataType(dataType);
        var.AddOnSetCallback(functor(*this, &CTrackViewKeyUIControls::OnInternalVariableChange));
        varArray.AddVariable(&var);
        m_registeredVariables.push_back(&var);
    }
    //////////////////////////////////////////////////////////////////////////
    void AddVariable(CVariableBase& var, const char* varName, unsigned char dataType = IVariable::DT_SIMPLE)
    {
        if (varName)
        {
            var.SetName(varName);
        }
        var.SetDataType(dataType);
        var.AddOnSetCallback(functor(*this, &CTrackViewKeyUIControls::OnInternalVariableChange));
        m_pVarBlock->AddVariable(&var);
        m_registeredVariables.push_back(&var);
    }
    void OnInternalVariableChange(IVariable* pVar);

protected:
    _smart_ptr<CVarBlock> m_pVarBlock;
    std::vector<_smart_ptr<IVariable> > m_registeredVariables;
    CTrackViewKeyPropertiesDlg* m_pKeyPropertiesDlg;
};

//////////////////////////////////////////////////////////////////////////
class CTrackViewTrackPropsDlg
    : public QWidget
{
    Q_OBJECT
public:
    CTrackViewTrackPropsDlg(QWidget* parent = 0);
    ~CTrackViewTrackPropsDlg();

    void OnSequenceChanged();
    bool OnKeySelectionChange(CTrackViewKeyBundle& keys);
    void ReloadKey();

protected:
    void SetCurrKey(CTrackViewKeyHandle& keyHandle);

protected slots:
    void OnUpdateTime();

protected:
    CTrackViewKeyHandle m_keyHandle;
    QScopedPointer<Ui::CTrackViewTrackPropsDlg> ui;
};


//////////////////////////////////////////////////////////////////////////
class TrackViewKeys;
class CTrackViewKeyPropertiesDlg
    : public QWidget
    , public ITrackViewSequenceListener
{
public:
    CTrackViewKeyPropertiesDlg(QWidget* hParentWnd);

    void SetKeysCtrl(CTrackViewDopeSheetBase* pKeysCtrl)
    {
        m_keysCtrl = pKeysCtrl;
        if (m_keysCtrl)
        {
            m_keysCtrl->SetKeyPropertiesDlg(this);
        }
    }

    void OnSequenceChanged();

    void PopulateVariables();
    void PopulateVariables(ReflectedPropertyControl* propCtrl);

    // ITrackViewSequenceListener
    virtual void OnKeysChanged(CTrackViewSequence* pSequence) override;
    virtual void OnKeySelectionChanged(CTrackViewSequence* pSequence) override;

protected:
    //////////////////////////////////////////////////////////////////////////
    void OnVarChange(IVariable* pVar);
    void CreateAllVars();
    void AddVars(CTrackViewKeyUIControls* pUI);
    void ReloadValues();

protected:
    std::vector< _smart_ptr<CTrackViewKeyUIControls> > m_keyControls;

    _smart_ptr<CVarBlock> m_pVarBlock;

    ReflectedPropertyControl* m_wndProps;
    CTrackViewTrackPropsDlg* m_wndTrackProps;

    CTrackViewDopeSheetBase* m_keysCtrl;

    CTrackViewTrack* m_pLastTrackSelected;
};

#endif // CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWKEYPROPERTIESDLG_H
