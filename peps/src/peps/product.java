package peps;

public class product {
	public equity barclays;
	public equity nokia;
	public fx EURUSD;
	public double[][] correl;
	
	public product() {
		barclays = new equity("barclays", 100, 0.3);
		nokia = new equity ("alcoa", 50, 0.4);
		EURUSD = new fx("EURUSD", 0.8, 0.1, "EUR", "USD");
		correl = new double[3][3];
		correl[0][0] = 1;
		correl[0][1] = 0.3;
		correl[0][2] = 0.2;
		correl[1][0] = 0.3;
		correl[1][1] = 1;
		correl[1][2] = 0.4;
		correl[2][0] = 0.2;
		correl[2][1] = 0.4;
		correl[2][2] = 1;
		}
}
