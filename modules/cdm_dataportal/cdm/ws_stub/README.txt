CDM Webservice Stub files
=========================================

ws_stub contains XML and JSON serialisations of eu.etaxonomy.cdm.dto objects its respective subfolders.

These serialisations can be used to drive test runs during development of cdm php applications.
If the drual vatiable 'cdm_webservice_url' is setup to point to the respective svn repository subfolder,
the cdm_api will use these files instead of sending requests to a real service. 

Naming convention for service stub files:
------------------------------------------
The CDM Webservice is queried with parametriesed URLs. 
The filename and query part of the URLs are translated into a filename without query part by the following 
replacement rules. It is not expectet that the service URLs will contain fragments, thus no according 
replacement rule is defined.

endcoding rules:
	? -> ;
	& -> ,
	
Additionally a fileextension is appended to the resulting filename which reflects the service
type (drupal variable 'cdm_webservice_type').

