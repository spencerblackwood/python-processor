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

#include <filesystem>

#include "PythonProcessor.h"
#include "PythonProcessorEditor.h"

namespace py = pybind11;

py::scoped_interpreter guard{};
py::gil_scoped_release release;

PythonProcessor::PythonProcessor()
    : GenericProcessor("Python Processor")
{
    scriptPath = "";
    isActive = false;
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
    importModule();
}

void PythonProcessor::process(AudioBuffer<float>& buffer)
{
    checkForEvents(true);

    py::gil_scoped_acquire acquire;

    if (isActive)
    {
        for (auto stream : getDataStreams())
        {

            if ((*stream)["enable_stream"])
            {

                const uint16 streamId = stream->getStreamId();

                const int numSamples = getNumSamplesInBlock(streamId);
                const int numChannels = stream->getChannelCount();

                // Only for blocks bigger than 0
                if (numSamples > 0) 
                {
                    py::array_t<float> numpyArray = py::array_t<float>({ numChannels, numSamples });

                    // Read into numpy array
                    for (int i = 0; i < numChannels; ++i) {
                        int globalChannelIndex = getGlobalChannelIndex(stream->getStreamId(), i);

                        const float* bufferChannelPtr = buffer.getReadPointer(globalChannelIndex);
                        float* numpyChannelPtr = numpyArray.mutable_data(i, 0);
                        memcpy(numpyChannelPtr, bufferChannelPtr, sizeof(float) * numSamples);
                    }

                    // Call python script
                    pyProcessorObject.attr("process")(numpyArray);


                    // Write from numpy array?
                    for (int i = 0; i < numChannels; ++i) {
                        int globalChannelIndex = getGlobalChannelIndex(stream->getStreamId(), i);

                        float* bufferChannelPtr = buffer.getWritePointer(globalChannelIndex);
                        const float* numpyChannelPtr = numpyArray.data(i, 0);
                        memcpy(bufferChannelPtr, numpyChannelPtr, sizeof(float) * numSamples);
                    }
                }
            
            }
        }
    }
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

bool PythonProcessor::startAcquisition() 
{
    if (isActive)
    {
        py::gil_scoped_acquire acquire;
        pyProcessorObject.attr("start_acquisition")();
        return true;
    }
    return false;
}

bool PythonProcessor::stopAcquisition() {
    if (isActive)
    {
        py::gil_scoped_acquire acquire;
        pyProcessorObject.attr("stop_acquisition")();
        return true;
    }
    return false;
}

void PythonProcessor::setScriptPath(String path)
{
    scriptPath = path;
}


bool PythonProcessor::importModule()
{

    if (scriptPath == "")
    {
        return false;
    }

    py::gil_scoped_acquire acquire;

    LOGC("Importing Python module from ", scriptPath.toRawUTF8());

    try
    {
        // Get module info (change to get from editor)
        std::filesystem::path path(scriptPath.toRawUTF8());
        std::string module_dir = path.parent_path().string();
        std::string file_name = path.filename().string();
        std::string module_name = file_name.substr(0, file_name.find_last_of("."));

        // Add module directory to sys.path
        py::module_ sys = py::module_::import("sys");
        py::object append = sys.attr("path").attr("append");
        append(module_dir);
        py::object pyProcessorClass = py::module_::import(module_name.c_str()).attr("PyProcessor");

        // Make processor object
        pyProcessorObject = pyProcessorClass();

        LOGC("Successfully imported ", module_name);

        isActive = true;
        return true;
    }

    catch (std::exception& exc)
    {
        LOGC("Failed to import Python module.");
        isActive = false;
        return false;
    }

}
