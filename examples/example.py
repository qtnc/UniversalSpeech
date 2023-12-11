from UniversalSpeech import *

# Enable native speech
setValue(ENABLE_NATIVE_SPEECH, True)

# Set the speech rate to 150 if it is supported
if getValue(RATE_SUPPORTED):
    setValue(RATE, 150)

print("Here is a list of supported engines : ")
print("{:<6}. {:<50}, {:>10}".format("ID", "Name", "Available"))

# Iterate through engines
i = 0
while True:
    name = getString(ENGINE + i)
    if name is None:
        break
    
    avail = getValue(ENGINE_AVAILABLE + i) != 0
    print("{:<6}. {:<50}, {:<10}".format(i, name, "yes" if avail else "no"))
    i += 1

# Get engine used
engine_id = getValue(ENGINE)
engine_used = getString(ENGINE + engine_id)
say("You are using " + engine_used)
print("You are using " + engine_used)