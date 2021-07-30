Disclaimer : This code is not mine ALONE. This repo is my sandbox.<br>
I am in charge of the network part.<br>
The rest of the project has been beautifully made by my team mates, @agogibus and @42f.<br>

07/30/2021 :<br>
    - Implentation of threads for each server in the config. Every thread actually pauses when they do not have any work and re activate when needed. This will allow good ressource sharing between the servers.<br>
    - Added kqueue system to monitor the activity on all servers ports and to wake up their threads when needed.<br>