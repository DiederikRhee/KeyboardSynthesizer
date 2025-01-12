import soundfile as sf
import sounddevice as sd

# Open the audio file
file_path = r"C:\Users\diede\Downloads\SalamanderGrandPianoV3+20161209_48khz24bit.tar\SalamanderGrandPianoV3+20161209_48khz24bit\SalamanderGrandPianoV3_48khz24bit\48khz24bit\A4v8.wav"
wf = sf.SoundFile(file_path)

def callback(outdata, frames, time, status):
    if status:
        print(f"Status: {status}")  # Display stream status warnings/errors
    
    # Read data from the file
    data = wf.buffer_read(frames, dtype='float32')
    
    # If we reach the end of the file, pad with zeros and stop playback
    if len(data) < len(outdata):
        outdata[:len(data)] = data
        outdata[len(data):] = 0  # Fill the rest with silence
        raise sd.CallbackStop  # Stop the stream gracefully
    else:
        outdata[:] = data  # Fill the output buffer with audio data

# Create and start the output stream
with sd.RawOutputStream(samplerate=wf.samplerate,
                        channels=wf.channels,
                        dtype='float32',
                        callback=callback) as stream:
    print("Playing audio...")
    stream.start()  # Start the audio stream
    while stream.active:
        pass  # Keep the program running until playback finishes

print("Playback finished.")
