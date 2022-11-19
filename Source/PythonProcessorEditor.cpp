/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2022 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PythonProcessorEditor.h"
#include "PythonProcessor.h"

ScriptPathButton::ScriptPathButton(Parameter* param) : ParameterEditor(param)
{
	utilButton = std::make_unique<UtilityButton>("...", Font(12));
	utilButton->addListener(this);
	addAndMakeVisible(utilButton.get());

	setBounds(0, 0, 20, 20);
}


void ScriptPathButton::buttonClicked(Button* label)
{
	FileChooser chooseScriptDirectory("Please select a python script...", File(CoreServices::getDefaultUserSaveDirectory()), "*.py");

	if (chooseScriptDirectory.browseForFileToOpen())
	{
		param->setNextValue(chooseScriptDirectory.getResult().getFullPathName());
	}
}

void ScriptPathButton::resized()
{
	utilButton->setBounds(0, 0, 20, 20);

}



PythonProcessorEditor::PythonProcessorEditor(PythonProcessor* parentNode) 
    : GenericEditor(parentNode)
{
	// Set ptr to parent
	pythonProcessor = parentNode;

    desiredWidth = 200;

	scriptPathLabel = new Label("Script Path Label", "No Module Loaded");
	scriptPathLabel->setTooltip(scriptPathLabel->getText());
	scriptPathLabel->setMinimumHorizontalScale(0.7f);
	scriptPathLabel->setJustificationType(Justification::centredRight);
	scriptPathLabel->setBounds(20, 35, 137, 20);
	scriptPathLabel->setColour(Label::backgroundColourId, Colours::grey);
	scriptPathLabel->setColour(Label::backgroundWhenEditingColourId, Colours::white);
	scriptPathLabel->setJustificationType(Justification::centredLeft);
	addAndMakeVisible(scriptPathLabel);

	Parameter* scriptPathPtr = getProcessor()->getParameter("script_path");
	addCustomParameterEditor(new ScriptPathButton(scriptPathPtr), 162, 35);

	reimportButton = new UtilityButton("Reimport", Font(12));
	reimportButton->setBounds(60, 80, 80, 30);
	reimportButton->addListener(this);
	addAndMakeVisible(reimportButton);

}

void PythonProcessorEditor::buttonClicked(Button* button)
{

	if (button == reimportButton)
	{
		pythonProcessor->importModule();
	}

}

void PythonProcessorEditor::setPathLabelText(String s)
{
	scriptPathLabel->setText(s, dontSendNotification);
	scriptPathLabel->setTooltip(s);
}


