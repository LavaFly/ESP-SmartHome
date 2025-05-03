# pcModule
alias pcOn="curl http://pc.local/on"

# lightingModule
alias lightOn="curl http://lighting.local/on"
alias lightOff="curl http://lighting.local/off"
alias lightp="curl http://lighting.local/raiseBrightness"
alias lightm="curl http://lighting.local/lowerBrightness"

# ledModule
alias showTime="curl http://led.local/showTime"
alias showTemperature="curl http://led.local/showTemperature"
alias showQuality="curl http://led.local/showQuality"
alias showCO2="curl http://led.local/showCO2"

# requires websocat
alias showText="websocat ws://led.local/ws"
