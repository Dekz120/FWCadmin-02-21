FWCadmin is a raw draw of firewalls secutiry policies automation. Here i'am trying to create some functions from Tuffin. Specifically automation of ip security policies. This projects allows to create data base(in txt files lol) of firewalls, their interfaces and interactions rules with other firewalls. Relying on this data base programm(rule_maker) can parse request ticket to define: -is legal requested interaction -is required configuration changing on firewalls. Where configuration should be changed.

In result programm returns an objects can be used for API requests. These request you'll be able to use for firewall configuration applying.
