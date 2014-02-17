package peps;

public class fx {
	public String name;
	public double spot;
	public double vol;
	public String curr1;
	public String curr2;
	public double[] historiq;
	
	public fx( String namep, double spotp, double volp, String curr1p, String curr2p) {
		name = namep;
		spot = spotp;
		vol = volp;
		curr1 = curr1p;
		curr2 = curr2p;
		historiq = new double[5];
	}

}
