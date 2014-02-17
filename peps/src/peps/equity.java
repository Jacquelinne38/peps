package peps;

public class equity {
	public String name;
	public double spot;
	public double vol;
	public double[] historiq;
	

	public equity(String namep, double spotp, double volp ) {
		name = namep;
		spot = spotp;
		vol = volp;
		historiq = new double[4];
	}
	
	public String getName() {
	    return this.name;
	}
	
	public double getspot() {
	    return this.spot;
	}

	public double getvol() {
	    return this.vol;
	}
}
