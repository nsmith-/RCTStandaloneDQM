import re, json
from rrapi import RRApi, RRApiError

# Default User app URL
URL  = "http://runregistry.web.cern.ch/runregistry/"

def main():
    try:

        # Construct API object
        api = RRApi(URL)

        # Print all metadata
        for t in api.tables('L1T'):
            print "Table: ", t
            print "Columns: "
            for col in api.columns('L1T', t) :
                print "    {name} ({type}) : {title}".format(**col)

        #if api.app == "user":
            #print api.data( workspace = 'GLOBAL', table = 'datasets',   template = 'csv',  filter = {'datasetState':'= OPEN'}, query = "{castor.status} = 'GOOD'")
            #print api.data('GLOBAL', 'runsummary', 'json', ['scalPresent', 'startTime', 'sequenceName'], {'datasetExists': '= true'})

    except RRApiError, e:
        print e

if __name__ == '__main__':
    main()
