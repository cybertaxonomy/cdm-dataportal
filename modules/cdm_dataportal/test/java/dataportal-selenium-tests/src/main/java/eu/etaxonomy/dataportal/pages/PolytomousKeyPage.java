package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

public class PolytomousKeyPage extends PortalPage {

	public static final Logger logger = Logger.getLogger(PolytomousKeyPage.class);

	private static String drupalPagePathBase = "cdm_dataportal/polytomousKey";

	private DataPortalContext context;

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

	private List<WebElement> keyTableRows;

	public PolytomousKeyPage(WebDriver driver, DataPortalContext context, UUID keyUuid) throws MalformedURLException {
		super(driver, context, keyUuid.toString());
		this.context = context;
	}

	/**
	 * @param driver
	 * @param context
	 * @throws Exception
	 */
	public PolytomousKeyPage(WebDriver driver, DataPortalContext context) throws Exception {
		super(driver, context);
	}

	public static class KeyLineData{

		String nodeNumber = null;
		String edgeText = null;
		LinkClass linkClass = null;
		String linkText = null;


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

		public KeyLineData(String nodeNumber, String edgeText, LinkClass linkClass, String linkText) {
			this.nodeNumber = nodeNumber;
			this.edgeText = edgeText;
			this.linkClass = linkClass;
			this.linkText = linkText;
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
		Assert.assertEquals("edge text", data.edgeText + "\n" + data.linkText, keyEntry.findElement(By.className("edge")).getText());
		WebElement linkContainer = keyEntry.findElement(By.className(data.linkClass.name()));
		WebElement link = linkContainer.findElement(By.tagName("a"));
		Assert.assertEquals("link text", data.linkText, link.getText());
		logger.info("testing " +  data.linkClass.name() + " : " + getInitialUrlBase() + ":" + link.getAttribute("href"));
		link.click();
		wait.until(new VisibilityOfElementLocated(By.id("container")));

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





}
