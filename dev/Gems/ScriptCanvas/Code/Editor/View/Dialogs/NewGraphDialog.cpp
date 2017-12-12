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
#include "NewGraphDialog.h"

#include <QLineEdit>
#include <QPushButton>

#include "Editor/View/Dialogs/ui_NewGraphDialog.h"



namespace ScriptCanvasEditor
{
    NewGraphDialog::NewGraphDialog(const QString& title, const QString& text, QWidget* pParent /*=nullptr*/)
        : QDialog(pParent)
        , ui(new Ui::NewGraphDialog)
        , m_text(text)
    {
        ui->setupUi(this);

        setWindowTitle(title);

        QObject::connect(ui->GraphName, &QLineEdit::returnPressed, this, &NewGraphDialog::OnOK);
        QObject::connect(ui->GraphName, &QLineEdit::textChanged, this, &NewGraphDialog::OnTextChanged);
        QObject::connect(ui->ok, &QPushButton::clicked, this, &NewGraphDialog::OnOK);
        QObject::connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);

        ui->ok->setEnabled(false);
    }

    void NewGraphDialog::OnTextChanged(const QString& text)
    {
        ui->ok->setEnabled(!text.isEmpty());
    }

    void NewGraphDialog::OnOK()
    {
        QString itemName = ui->GraphName->text();
        m_text = itemName.toLocal8Bit().constData();

        accept();
    }

    #include <Editor/View/Dialogs/NewGraphDialog.moc>
}