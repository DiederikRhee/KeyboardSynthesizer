from scipy.io import wavfile
from scipy.signal import resample
import numpy as np
import os
import re

folder = r"C:\Users\diede\Downloads\SalamanderGrandPianoV3+20161209_48khz24bit.tar\SalamanderGrandPianoV3+20161209_48khz24bit\SalamanderGrandPianoV3_48khz24bit\48khz24bit"

notes = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]

# Regular expression for filenames starting with C, D#, F#, or A and capturing the number before v16
pattern = re.compile(r"^(C|D#|F#|A)(\d+)v16\.wav$")

sampleSet = []
sampleSetKeys = []
testfiles = []

sampleRate = 48000

maxValue = 0

# Find matching files and extract the note and number
matching_files_with_details = []
for root, dirs, files in os.walk(folder):
    for file in files:
        match = pattern.match(file)
        if match:
            note = match.group(1)  # Extract the note (e.g., C, D#, F#)
            octave = match.group(2)  # Extract the number before v16

            # Calculate the note key based on the octave and note index
            noteIndex = notes.index(note)
            octaveIndex = int(octave)
            noteKey = (octaveIndex - 2) * len(notes) + noteIndex


            print (file, noteKey)

            sampleRate, data = wavfile.read(os.path.join(folder, file))

            combined = np.average(data, axis=1)
            sampleSet.append(combined)
            sampleSetKeys.append(noteKey)
            testfiles.append(file)

            maxValue = max(maxValue, abs(combined.max()))
            maxValue = max(maxValue, abs(combined.min()))

sampleSetKeys = np.array(sampleSetKeys)
sortedIndexes = np.argsort(sampleSetKeys)

# Functie om een sample te interpoleren naar een nieuwe frequentie
def pitch_shift(sample, original_note, target_note):
    # Bereken het pitch-shift factor
    semitone_ratio = 2 ** ((target_note - original_note) / 12)
    new_length = int(len(sample) / semitone_ratio)
    # Gebruik resample om de pitch te veranderen
    return resample(sample, new_length)


def write_wave_file(data, samplerate, name):
    scale = np.iinfo(np.int16).max 
    scaled_data = np.int16(data * scale)  # Convert to 16-bit PCM
    wavfile.write(name, samplerate, scaled_data)

def write_data_file(data, name):
    data = data.astype(np.float32)
    with open(name, "wb") as f:
        data.tofile(f)

for noteKey in range (61):
    sortedindices = np.argsort(np.abs(sampleSetKeys - noteKey))

    sample = sampleSet[sortedindices[0]] / maxValue
    if (sampleSetKeys[sortedindices[0]] != noteKey):
        sample = pitch_shift(sample, sampleSetKeys[sortedindices[0]], noteKey)
    #write_wave_file(sample, sampleRate, f"{noteKey}.wav")
    write_data_file(sample, f"samples_piano/{noteKey}.bin")


 
