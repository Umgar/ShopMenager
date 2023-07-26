Client identification:
	Client needs to connect with the server and has 1 second to send an identyfication messege containing token generatated by the server at the first authorization.
Adding a client/creating client token:
	At the first attempt to connect client has 1 second to send an adding request, the server administrator is then inform of the user attempt and has to manualy add the user to the valid list. Then the server will create a user token and send it over to the new client.

After verification:
	After verification server will send a messege to the client containing information about every furniture location and products indexes located on this furniture.
	Format of the messege:
	# - end of messege
	; - end of furniture info
	: - end of furniture location info/start of the index list
	, - next value
	Example:
	"x1,y1,x2,y2:12345,23456,34567;x1,y1,x2,y2:54321,65432,76543;#"

