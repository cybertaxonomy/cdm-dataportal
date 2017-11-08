package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.UrlLoaded;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

public class PolytomousKeyPage extends PortalPage {

	public static final Logger logger = Logger.getLogger(PolytomousKeyPage.class);

	private static String drupalPagePathBase = "cdm_dataportal/polytomousKey";

	/* (non-Javadoc)
	 * @see eu.etaxonomy.dataportal.pages.PortalPage#getDrupalPageBase()
	 */
	@Override
	protected String getDrupalPageBase() {
		return drupalPagePathBase;
	}

	@FindBy(className="polytomousKey")
	@CacheLookup
	private WebElement keyTable;

	@FindBy(css="#identificationKey .sources span.reference")
	@CacheLookup
	private List<WebElement> sourceReferences;

	@FindBy(css="#identificationKey .annotations")
    @CacheLookup
    private WebElement annotations;

	private List<WebElement> keyTableRows;

	public PolytomousKeyPage(WebDriver driver, DataPortalContext context, UUID keyUuid) throws MalformedURLException {
		super(driver, context, keyUuid.toString());
	}

	public PolytomousKeyPage(WebDriver driver, DataPortalContext context) throws Exception {
		super(driver, context);
	}

	public String getKeyAnnotationsText(){
	    return annotations.getText();
	}

	public List<BaseElement> getSources() {
	    List<BaseElement> baseElements = new ArrayList<BaseElement>();
//	    List<WebElement> references = sources.findElements(By.className("reference"));
	    for(WebElement sr : sourceReferences) {
	        baseElements.add(new BaseElement(sr));
	    }
	    return baseElements;
	}

	public static class KeyLineData{

		String nodeNumber = null;
		String edgeText = null;
		LinkClass linkClass = null;
		String linkText = null;
        String suffix = "";


		public String getNodeNumber() {
			return nodeNumber;
		}

		public String getEdgeText() {
			return edgeText;
		}

		public LinkClass getLinkClass() {
			return linkClass;
		}

		public String getLinkText() {
			return linkText;
		}

		public String getLinkTextWithSuffix() {
            return linkText + (suffix != null ? suffix : "");
        }

		public KeyLineData(String nodeNumber, String edgeText, LinkClass linkClass, String linkText) {
			this.nodeNumber = nodeNumber;
			this.edgeText = edgeText;
			this.linkClass = linkClass;
			this.linkText = linkText;
		}

		/**
		 * @param suffix In cases where the linkText is a taxonName the link may be suffixed with the nomenclatural reference.
		 */
		public KeyLineData(String nodeNumber, String edgeText, LinkClass linkClass, String linkText, String suffix) {
            this.nodeNumber = nodeNumber;
            this.edgeText = edgeText;
            this.linkClass = linkClass;
            this.linkText = linkText;
            this.suffix = suffix;
        }
	}

	public enum LinkClass {
		nodeLinkToNode,
		nodeLinkToTaxon;
	}

	public PortalPage followPolytomousKeyLine(int lineIndex, KeyLineData data) throws Exception {

		keyTableRows = keyTable.findElements(By.xpath("tbody/tr"));
		WebElement keyEntry = keyTableRows.get(lineIndex);
		Assert.assertEquals("node number", data.nodeNumber, keyEntry.findElement(By.className("nodeNumber")).getText());
		Assert.assertEquals("edge text", composeFullEdgeText(data), keyEntry.findElement(By.className("edge")).getText());
		WebElement linkContainer = keyEntry.findElement(By.className(data.linkClass.name()));
		WebElement link = linkContainer.findElement(By.tagName("a"));
		Assert.assertEquals("link text", data.linkText, link.getText());
		String linkUrl = link.getAttribute("href");

		logger.info("clicking on " +  data.linkClass.name() + " : " + linkUrl);

		// click and wait
		link.click();
		wait.until(new AllTrue(new UrlLoaded(linkUrl), new VisibilityOfElementLocated(By.id("container"))));

		PortalPage nextPage = null;
		if(data.linkClass.equals(LinkClass.nodeLinkToTaxon)){
			nextPage = new TaxonProfilePage(driver, context);
		} else {
			// must be PolytomousKeyPage then
			if( !isOnPage()){
				nextPage = new PolytomousKeyPage(driver, context);
			} else {
				nextPage = this;
			}
		}

		return nextPage;
	}


    private String composeFullEdgeText(KeyLineData data) {
        return data.edgeText + "\n" + data.getLinkTextWithSuffix();
    }





}
