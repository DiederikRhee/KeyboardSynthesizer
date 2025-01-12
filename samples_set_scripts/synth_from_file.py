import umidiparser
import numpy as np
import threading
import sounddevice as sd
from typing import List

velocityScale = np.array([i / 127 for i in range(128)])

class KeySample:
    def __init__(self, aKeyIndex):
        self.keyIndex = aKeyIndex
        self.samples = None
        self.currentIndex = 0
        self.isPlaying = False

    def loadFromFile(self, filename):
        self.samples = np.fromfile(filename, dtype=np.float32)

    def NoteOn(self):
        if (self.isPlaying): return

        self.currentIndex = 0
        self.isPlaying = True

    def NoteOff(self):
        self.isPlaying = False
        self.currentIndex = 0

    def Process(self):
        if (not self.isPlaying):
            return 0
        
        self.currentIndex += 1
        if (self.currentIndex >= len(self.samples)):
            self.isPlaying = False
        
        return self.samples[self.currentIndex]


framesPerUs = 48000 / 1000000

Keys : List[KeySample] = [KeySample(i) for i in range(61)]
for key in (Keys):
    key.loadFromFile(f"samples_piano/{key.keyIndex}.bin")

midiFile = umidiparser.MidiFile("mond_2_format0.mid")
for event in midiFile:
    frames = int(round(event.delta_us * framesPerUs))
    if (frames > 0):
        newData = np.zeros(frames, dtype=np.float32)
        for frame in range(frames):
            for key in (Keys):
                newData[frame] += key.Process()

        q = 0

    if (event.status == umidiparser.NOTE_ON or event.status == umidiparser.NOTE_OFF):
        key = event.note - 36
        if (key >= 0 and key < len(Keys)):
            if (event.status == umidiparser.NOTE_ON and event.velocity > 0):
                Keys[key].NoteOn()
            else:
                Keys[key].NoteOff()








