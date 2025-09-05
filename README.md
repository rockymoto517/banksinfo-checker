# Credit
Thanks to [this old project](https://github.com/simon-wh/DieselEngineFormats/blob/master/DieselEngineFormats/BNK/BanksInfo.cs) for all of the heavylifting of reversing the binary format; I just put the data to code.

# Building
Standard cmake build:
```
cmake -B {build_folder}
```

Add `-DCMAKE_RELEASE_TYPE=release` if needed and change the build file generator `-G` as needed. 

(This was developed using `g++`.)

## Examples
```
$ ./bnksinfo -D existing_banks.banksinfo
Building bank info...

Metadata:
        file_size: 3370888 bytes
        num_banks: 505
        banks_offset: 64
        num_sounds: 80074
        sounds_offset: 17248
        num_sound_lookups: 80074
        sound_lookups_offset: 1298432

$ ./bnksinfo -l existing_banks.banksinfo
Building bank info...

Soundbanks:
        core/soundbanks/core
        ...
        soundbanks/weapon_xeno
        soundbanks/weapon_zsniper
        soundbanks/zipline
```
