# pcModule
alias pcOn="curl http://pcModule.local/pcPowerOn"

# lightingModule
alias lightOn="curl http://lightingModule.local/lightingOn"
alias lightOff="curl http://lightingModule.local/lightingOff"
alias lightp="curl http://lightingModule.local/raiseBrightness"
alias lightm="curl http://lightingModule.local/lowerBrightness"

# ledModule
alias showTime="curl http://ledModule.local/showTime"
alias showTemperature="curl http://ledModule.local/showTemperature"
alias showQuality="curl http://ledModule.local/showQuality"
alias showCO2="curl http://ledModule.local/showCO2"

# requires websocat
alias showText="websocat ws://ledModule.local/ws"
