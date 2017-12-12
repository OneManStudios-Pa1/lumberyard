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

#include <AzCore/Component/EntityId.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/string/string.h>

#include <QGraphicsWidget>

#include <Styling/StyleHelper.h>
#include <Types/TranslationTypes.h>

namespace GraphCanvas
{
    //! The GraphCanvasLabel gives a QGraphicsWidget that is able to display text, and be placed into a layout.
    class GraphCanvasLabel
        : public QGraphicsWidget
    {
    public:
        AZ_CLASS_ALLOCATOR(GraphCanvasLabel, AZ::SystemAllocator, 0);

        GraphCanvasLabel(QGraphicsItem* parent = nullptr);
        ~GraphCanvasLabel() = default;

        void SetLabel(const AZStd::string& label, const AZStd::string& translationContext = AZStd::string(), const AZStd::string& translationKey = AZStd::string());
        void SetLabel(const TranslationKeyedString& value);
        AZStd::string GetLabel() const { return AZStd::string(m_labelText.toStdString().c_str()); }

        void SetSceneStyle(const AZ::EntityId& sceneId, const char* style);
        void SetStyle(const AZ::EntityId& entityId, const char* styleElement);
        void RefreshDisplay();

        //! Sets whether the text should elide if it grows beyond max-width
        //! (Note: currently incompatible with word wrap)
        void SetElide(bool elide);

        //! Sets whether the next should wrap if it grows beyond max-width
        //! (Note: currently incompatible with text elide)
        void SetWrap(bool wrap);

        void SetDefaultAlignment(Qt::Alignment defaultAlignment);

        Styling::StyleHelper& GetStyleHelper();
        const Styling::StyleHelper& GetStyleHelper() const;

    protected:
        // QGraphicsItem
        bool event(QEvent* qEvent) override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;             

        // QGraphicsLayoutItem
        QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;
        ////

    private:

        void UpdateDisplayText();
        void UpdateDesiredBounds();

        Qt::Alignment   m_defaultAlignment;
        bool m_elide;
        bool m_wrap;

        QString         m_labelText;
        QString         m_displayText;

        QSizeF          m_maximumSize;
        QSizeF          m_minimumSize;

        //! This is influenced by the reflected value m_title.
        //! That's why we must update this value when m_title changes.
        QRectF m_desiredBounds;

        Styling::StyleHelper m_styleHelper;
    };
}