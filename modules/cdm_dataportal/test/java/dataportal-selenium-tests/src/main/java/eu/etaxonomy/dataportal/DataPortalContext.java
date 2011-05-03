package eu.etaxonomy.dataportal;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.UUID;

public enum DataPortalContext {
	
	cichorieae(
			"http://160.45.63.201/dataportal/preview/cichorieae/",
			"http://127.0.0.1:8080",
			"534e190f-3339-49ba-95d9-fa27d5493e3e"),
	palmae(
			"http://160.45.63.201/dataportal/preview/palmae/",
			"http://127.0.0.1:8080",
			"534e190f-3339-49ba-95d9-fa27d5493e3e");
	//floraMalesiana;
	
	URI baseUri;
	URI cdmServerUri;
	UUID classificationUUID;
	String themeName;

	
	private DataPortalContext(String baseUri, String cdmServerUri,
			String classificationUUID) {
		try {
			this.baseUri = new URI(baseUri);
		} catch (URISyntaxException e) {
			e.printStackTrace();
		}
		try {
			this.cdmServerUri = new URI(cdmServerUri);
		} catch (URISyntaxException e) {
			e.printStackTrace();
		}
		this.classificationUUID = UUID.fromString(classificationUUID);
	}


	public URI getBaseUri() {
		return baseUri;
	}


	public URI getCdmServerUri() {
		return cdmServerUri;
	}


	public UUID getClassificationUUID() {
		return classificationUUID;
	}
	
	
	
	
}
