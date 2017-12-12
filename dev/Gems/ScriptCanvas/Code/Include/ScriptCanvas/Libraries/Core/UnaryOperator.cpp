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

#include "precompiled.h"
#include "UnaryOperator.h"

namespace ScriptCanvas
{
    namespace Nodes
    {
        const char* UnaryOperator::k_evaluateName = "In";
        const char* UnaryOperator::k_onTrue = "True";
        const char* UnaryOperator::k_onFalse = "False";

        const char* UnaryOperator::k_valueName = "Value";
        const char* UnaryOperator::k_resultName = "Result";

        Datum UnaryOperator::Evaluate(const Datum& value)
        {
            AZ_Assert(false, "Evaluate must be overridden");
            return Datum();
        };

        void UnaryOperator::OnInputSignal(const SlotId& slot)
        {
            AZ_Assert(false, "OnInputSignal must be overridden");
        }

        void UnaryOperator::ConfigureSlots()
        {
            AddSlot(UnaryOperator::k_evaluateName, "Signal to perform the evaluation when desired.", ScriptCanvas::SlotType::ExecutionIn);
        }

        void UnaryOperator::Reflect(AZ::ReflectContext* reflection)
        {
            if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
            {
                serializeContext->Class<UnaryOperator, Node>()
                    ->Version(1)
                    ;

                if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                {
                    editContext->Class<UnaryOperator>("UnaryOperator", "UnaryOperator")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ;
                }
            }
        }

        void UnaryExpression::InitializeUnaryExpression()
        {
        }

        void UnaryExpression::OnInputSignal(const SlotId& slot)
        {
            AZ_Assert(m_outputSlotIndex != -1, "m_outputSlotIndex was not configured to the index of the result slot");
            const Datum output = Evaluate(m_inputData[k_datumIndex]);
            PushOutput(output, m_slotContainer.m_slots[m_outputSlotIndex]);

            const bool* value = output.GetAs<bool>();
            if (value && *value)
            {
                SignalOutput(GetSlotId(k_onTrue));
            }
            else
            {
                SignalOutput(GetSlotId(k_onFalse));
            }
        }

        void UnaryExpression::ConfigureSlots()
        {
            AddInputDatumSlot(UnaryExpression::k_valueName, "", AZStd::move(Data::Type::Boolean()), Datum::eOriginality::Original);

            SlotId slotId = AddOutputTypeSlot(UnaryExpression::k_resultName, "", Data::Type::Boolean(), OutputStorage::Optional);
            const Slot* slot{};
            GetValidSlotIndex(slotId, slot, m_outputSlotIndex);

            UnaryOperator::ConfigureSlots();

            AddSlot(k_onTrue, "Signaled if the result of the operation is true.", ScriptCanvas::SlotType::ExecutionOut);
            AddSlot(k_onFalse, "Signaled if the result of the operation is false.", ScriptCanvas::SlotType::ExecutionOut);

            InitializeUnaryExpression();

        }

        void UnaryExpression::Reflect(AZ::ReflectContext* reflection)
        {
            if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
            {
                serializeContext->Class<UnaryExpression, UnaryOperator>()
                    ->Version(0)
                    ;

                if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                {
                    editContext->Class<UnaryExpression>("UnaryExpression", "UnaryExpression")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ;
                }
            }
        }
    }
}

