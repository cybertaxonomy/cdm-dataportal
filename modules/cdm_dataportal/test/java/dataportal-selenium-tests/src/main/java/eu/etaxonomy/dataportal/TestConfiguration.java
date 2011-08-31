/**
 *
 */
package eu.etaxonomy.dataportal;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Properties;
import java.util.UUID;

import org.apache.log4j.Logger;

import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.selenium.WebDriverFactory;

/**
 * @author a.kohlbecker
 *
 */
public class TestConfiguration {

	/**
	 *
	 */
	private static final String DATA_PORTAL_TEST_PROPERTIES_FILE = "DataPortalTest.xml";

	public final static Logger logger = Logger.getLogger(TestConfiguration.class);

	final Properties properties = new Properties();

	private URL propertySourceUri;

	public Properties getProperties() {
		return properties;
	}

	private static TestConfiguration testConfiguration = null;

	private TestConfiguration() {
		String userHome = System.getProperty("user.home");
		if (userHome != null) {
			File propertiesFile = new File(userHome, ".cdmLibrary" + File.separator + DATA_PORTAL_TEST_PROPERTIES_FILE);

			try {

				InputStream in;
				if (propertiesFile.exists()) {
					propertySourceUri = propertiesFile.toURI().toURL();
					in = new FileInputStream(propertiesFile);
				} else {
					String resourceName = "/eu/etaxonomy/dataportal/"+ DATA_PORTAL_TEST_PROPERTIES_FILE;

					in =  this.getClass().getResourceAsStream(resourceName);
					propertySourceUri = this.getClass().getResource(resourceName);
				}
				logger.info("Loading test configuration from " + propertySourceUri);
				properties.loadFromXML(in);
				in.close();

				updateSystemProperties(false);

			} catch (FileNotFoundException e) {
				logger.error(e);
			} catch (IOException e) {
				logger.error(e);
			}

		}
	}

	/**
	 *
	 */
	private void updateSystemProperties(boolean doOverride) {
		for(Object o : properties.keySet()){
			String key = (String)o;

			// update all webdriver properties and the browser property
			if(key.startsWith("webdriver.") || key.equals(WebDriverFactory.SYSTEM_PROPERTY_NAME_BROWSER)){
				if(doOverride || System.getProperty(key) == null){
					System.setProperty(key, properties.getProperty(key));
				}
			}
		}

	}

	public static String getProperty(String key) throws TestConfigurationException{
		return getProperty(key, String.class, false);
	}

	@SuppressWarnings("unchecked")
	public static <T> T getProperty(String key, Class<T> type, boolean nonNull) throws TestConfigurationException{
		if(testConfiguration == null){
			testConfiguration = new TestConfiguration();
		}
		String value = testConfiguration.getProperties().getProperty(key);


		if(value != null){
			if(URI.class.isAssignableFrom(type)){
				try {
					return (T) new URI(value);
				} catch (URISyntaxException e) {
					throw new TestConfigurationException("Invalid URI " + value + " in property " + key + " of " + testConfiguration.propertySourceUri.toString(), e);
				}
			} else if(UUID.class.isAssignableFrom(type)){
				try {
					return (T) UUID.fromString(value);
				} catch (IllegalArgumentException e) {
					throw new TestConfigurationException("Invalid UUID " + value + " in property " + key + " of " + testConfiguration.propertySourceUri.toString(), e);
				}
			} else if(String.class.isAssignableFrom(type)){
				return (T) value;
			} else {
				throw new TestConfigurationException("Unsupported type " + type.toString());
			}
		} else {
			if( nonNull) {
				throw new TestConfigurationException("Property " + key + " of " + testConfiguration.propertySourceUri.toString() + " must not be null");
			}
		}

		return null;
	}

	public static void main(String[] args) {
		String userHome = System.getProperty("user.home");
		TestConfiguration.logger.error(userHome);
	}

}
