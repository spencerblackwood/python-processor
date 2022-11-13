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

PythonProcessorEditor::PythonProcessorEditor(PythonProcessor* parentNode) 
    : GenericEditor(parentNode)
{
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

	scriptPathButton = new UtilityButton("...", Font(12));
	scriptPathButton->setBounds(162, 35, 18, 20);
	scriptPathButton->addListener(this);
	addAndMakeVisible(scriptPathButton);

	reimportButton = new UtilityButton("Reimport", Font(12));
	reimportButton->setBounds(60, 80, 80, 30);
	reimportButton->addListener(this);
	addAndMakeVisible(reimportButton);

}

void PythonProcessorEditor::buttonClicked(Button* button)
{
	if (button == scriptPathButton)
	{
		FileChooser chooseScriptDirectory("Please select the python script...", File(CoreServices::getDefaultUserSaveDirectory()), "*.py");

		if (chooseScriptDirectory.browseForFileToOpen())
		{
			String new_path = chooseScriptDirectory.getResult().getFullPathName();

			
			pythonProcessor->setScriptPath(new_path);
			scriptPathLabel->setText("Importing", juce::NotificationType::dontSendNotification);

			bool success = pythonProcessor->importModule();
			scriptPathLabel->setTooltip(new_path);

			if (success)
			{
				String file_name = new_path.fromLastOccurrenceOf("\\", false, false);

				scriptPathLabel->setText(file_name, juce::NotificationType::dontSendNotification);
			}
			else
			{
				scriptPathLabel->setText("No Module Loaded", juce::NotificationType::dontSendNotification);
			}
		}
	}

	if (button == reimportButton)
	{
		pythonProcessor->importModule();
	}


}