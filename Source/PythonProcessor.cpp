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

#include <pybind11/embed.h>
#include <pybind11/numpy.h>

#include "PythonProcessor.h"
#include "PythonProcessorEditor.h"

namespace py = pybind11;

py::scoped_interpreter guard{};
py::gil_scoped_release release;

PythonProcessor::PythonProcessor()
    : GenericProcessor("Python Processor")
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
    // Do this later...
    //addTTLChannel("Python Processor Output");

    py::gil_scoped_acquire acquire;

    // Get module info (change to get from editor)
    char* module_dir = "c:/users/blackwood.s/documents/python scripts";
    char* module_name = "py_processor";

    // Add module directory to sys.path
    py::module_ sys = py::module_::import("sys");
    py::object append = sys.attr("path").attr("append");
    append(module_dir);
    py::object Processor = py::module_::import(module_name).attr("PyProcessor");

    // Make processor object
    pyProcessor = Processor();
}


void PythonProcessor::process(AudioBuffer<float>& buffer)
{
    checkForEvents(true);

    py::gil_scoped_acquire acquire;

    for (auto stream : getDataStreams())
    {

        if ((*stream)["enable_stream"])
        {

            const uint16 streamId = stream->getStreamId();

            const int numSamples = getNumSamplesInBlock(streamId);
            const int numChannels = stream->getChannelCount();

            py::array_t<float> numpyArray = py::array_t<float>({ numChannels, numSamples });

            // Read into numpy array
            for (int i = 0; i < numChannels; ++i) {
                int globalChannelIndex = getGlobalChannelIndex(stream->getStreamId(), i);

                const float* bufferChannelPtr = buffer.getReadPointer(globalChannelIndex);
                float* numpyChannelPtr = numpyArray.mutable_data(i, 0);
                memcpy(numpyChannelPtr, bufferChannelPtr, sizeof(float) * numSamples);
            }

            // Call python script
            pyProcessor.attr("process")(numpyArray);

            // Write from numpy array?
            for (int i = 0; i < numChannels; ++i) {
                int globalChannelIndex = getGlobalChannelIndex(stream->getStreamId(), i);

                float* bufferChannelPtr = buffer.getWritePointer(globalChannelIndex);
                const float* numpyChannelPtr = numpyArray.data(i, 0);
                memcpy(bufferChannelPtr, numpyChannelPtr, sizeof(float) * numSamples);
            }


            //    numpyArray = py::array_t<float>(
            //        { numChannels, numSamples }, // Dimensions
            //        { sizeof(float) * bufferSize, sizeof(float) }, // Strides
            //        firstChannelPtr); // Pointer
            //}
            //else if (numChannels == 1) {
            //    numpyArray = py::array_t<float>(
            //        { numSamples }, // Dimensions
            //        { sizeof(float) }, // Strides
            //        firstChannelPtr); // Pointer
            //}
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
