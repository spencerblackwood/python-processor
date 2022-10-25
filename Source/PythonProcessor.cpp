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

#include "PythonProcessor.h"

#include "PythonProcessorEditor.h"


PythonProcessor::PythonProcessor()
    : GenericProcessor("Plugin Name")
{

}


PythonProcessor::~PythonProcessor()
{

}


AudioProcessorEditor* PythonProcessor::createEditor()
{
    editor = std::make_unique<PythonProcessorEditor>(this);
    return editor.get();
}


void PythonProcessor::updateSettings()
{


}


void PythonProcessor::process(AudioBuffer<float>& buffer)
{

    checkForEvents(true);

}


void PythonProcessor::handleTTLEvent(TTLEventPtr event)
{

}


void PythonProcessor::handleSpike(SpikePtr event)
{

}


void PythonProcessor::handleBroadcastMessage(String message)
{

}


void PythonProcessor::saveCustomParametersToXml(XmlElement* parentElement)
{

}


void PythonProcessor::loadCustomParametersFromXml(XmlElement* parentElement)
{

}
