# Python Processor

**THIS BRANCH IS NO LONGER SUPPORTED. SEE THE OFFICIAL FORK HERE:** 
https://github.com/open-ephys-plugins/python-processor


A plugin for adding custom Python scripts to the [Open Ephys GUI](https://github.com/open-ephys/plugin-GUI) signal chain. 

## Build Notes

So far the build process has only been tested on Windows. The plugin uses [pybind11](https://pybind11.readthedocs.io/en/stable/) to call Python functions from the GUI. Right now it seems to require a version of Python be available on the Windows PATH variable at compile time and at run time. Modules available in sys.path can be imported.

## Usage

You must have numpy installed and available on sys.path in order to run the processor. Each plugin node creates an instance of a user-defined Python class named PyProcessor. Edit the template available in the Modules folder in this repo, then load the .py module using the file dialog in the plugin node. The reload button will reimport the module if you make edits.



