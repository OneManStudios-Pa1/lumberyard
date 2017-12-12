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

#include <AzQtComponents/AzQtComponentsAPI.h>
#include <QRect>

class QWidget;
class QMainWindow;
class QPainter;

namespace AzQtComponents
{

    AZ_QT_COMPONENTS_API QRect GetTotalScreenGeometry();
    AZ_QT_COMPONENTS_API void EnsureWindowWithinScreenGeometry(QWidget* widget);

    AZ_QT_COMPONENTS_API void SetClipRegionForDockingWidgets(QWidget* widget, QPainter& painter, QMainWindow* mainWindow);
    
    AZ_QT_COMPONENTS_API void SetCursorPos(const QPoint& point);
    AZ_QT_COMPONENTS_API void SetCursorPos(int x, int y);

} // namespace AzQtComponents

