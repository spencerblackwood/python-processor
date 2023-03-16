import numpy as np

class PyProcessor:
    
    # A new processor is initialized whenever the plugin settings are updated
    def __init__(self, num_channels, sample_rate):
        pass
    
    # Process each data buffer. Data is a numpy array. Return events as an array of (sample, line, state) tuples
    def process(self, data):

        # example: return [(0,0,True), (100,0,False)]
        return []
        
    # Called at start of acquisition
    def start_acquisition(self):
        pass
    
    # Called when acquisition is stopped
    def stop_acquisition(self):
        pass
    
    # Respond to TTL events
    def handle_ttl_event(self, state, sample_number, channel, line, stream_id):
        pass
    
    # Respond to spike events
    def handle_spike_event(self):
        pass
    
    # Called when recording starts
    def start_recording(self, recording_dir):
        pass
    
    # Called when recording stops
    def stop_recording(self):
        pass