#!/usr/bin/expect

set timeout 300 

set GITVERSION [lindex $argv 0]
send_user "GITVERSION:$GITVERSION\n"

spawn git checkout $GITVERSION

spawn git fetch
expect {
    "Password:" { send "zm19840501\r" }
}
interact  

spawn git pull
expect {
    "Password:" { send "zm19840501\r" }
}
interact  
