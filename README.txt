# Typaste by katahiromz

## What's this?

Typing + Paste == Typaste.

Typaste is a program that intercepts the pasting process by Ctrl+V
while this window is displayed and emulates text input like typing.
This allows you to break through the paste prohibition.

You can stop input by presssing Esc key.

License: MIT

## Operation

### Delay

- The delay used for key depress, hold, and release is the Travel delay 
  as configured with the Settings dialog.
- The delay between character key presses is randomly chosen between the
  two Delay values configured with the Settings dialog.

### Sound files

Typaste supports playing sound files when simulating key presses. 
The following applies:

- `enter.wav`, if available, is played when Enter is simulated.
- `space.wav`, if available, is played when the space key is simulated.
- `modifierdown.wav`, if available, is played when the depress of a
   modifier key (shift, control, alt) is simulated.
- `modifierup.wav`, if available, is played when the release of a
   modifier key (shift, control, alt) is simulated.
- If any of the previously mentioned sound files don't exist, the sound
  for a "regular" key press is used.
- All files with the extension `.wav` that were not previously mentioned
  are considered potential sound files for regular key presses.
- Using the configuration set with the Settings dialog, either of the
  following can be chosen:
  - one of the potential key press sound files is selected for all key 
    presses.
  - for each simulated key press, one of the potential key press sound
    files is randomly chosen.

## Supported Platforms

It works on Windows XP/Vista/7/10/11.

## Warning

Please do not use this software for illegal purposes.

## History

- 2019.08.07 ver.0.5.1
  - First release.
- 2019.08.14 ver.0.5.2
  - Add "Open ReadMe".
- 2019.XX.YY ver.0.5.3
  - Check window existence before startup.
  - Add play sound feature.
- 2022.08.28 ver.0.6.0-rvb
  - Resemble natural typing.
- 2022.09.17 ver.0.6.1-rvb
  - Add ability to inject simulated typos.
- 2022.09.17 ver.0.6.2-rvb
  - Address type conversion compiler warnings on 64-bit builds

## Contact Us

- Katayama Hirofumi MZ (katahiromz)
  katayama.hirofumi.mz@gmail.com
- Rutger van Bergen (rbergen), for changes implemented from version 0.6.0-rvb onwards
  rbergen@xs4all.nl
