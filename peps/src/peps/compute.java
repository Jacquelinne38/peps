package peps;

import java.util.Date;
import java.util.Random;


public class compute {
	public static void main(String[] args) {
	
			
		// pour le moment le drift est mal gere
		// les dates sont mal gerees
		// on ne gere pas les differents monnaies
		// dans ce premiet modele on considere alcoa en USD et barclays en EUR
		// a quel moment doit on passer le spot en EUR
		// pour le moment il n 'y a pas le payoff
		// pour le moment il n' y a pas l'actualisation
		// pour le moment, il n'y a pas la performance lissee.
		// comment gérer la maturité
		// faire attention a diffuser en CZK mais que la valeur des actifs soient stockées avec leur monnaie respective
		// les taux de change bouge a chaque calcul il faut prendre le nouveau taux
		
		product prodStruc = new product();
		double maturity = 4 * 365;
		// on va devoir creer une fonction qui determine le dt pour t
		double dt = 365 / maturity;
		Random rand = new Random(new Date().getTime());
		int nbIt = 10000;
		double[] drift = {0.05, 0.05, 0.05};
		double[] spot = {prodStruc.barclays.spot, prodStruc.nokia.spot, prodStruc.EURUSD.spot} ;
		double[] vol = {prodStruc.barclays.vol, prodStruc.nokia.vol, prodStruc.EURUSD.vol};
		double[] compoExpo = {0,0,0};
		// boucle de monte carlo
		for (int i = 0; i < nbIt ; i++){
			double[] finalspot = spot;
			// ici peut etre doit on se souvenir du taux
			// boucle sur les 4 dt
			for (int j = 0; j < 4; j++){
				// il faut que je calcul chacune des composantes
				for (int k =  0 ; k < spot.length; k++){
					// pour le moment on en met pas la matrice de correlation les actifs sont donc non correles
					// ici il faut faire une truc avec les taux mais je vois pas trop
					compoExpo[k] = Math.exp((drift[k] - (Math.pow(vol[k], 2)/2))*dt + vol[k] * Math.sqrt(dt) * rand.nextGaussian());
					finalspot[k] *= compoExpo[k];
				}
				System.out.println("composante du spot en");
				System.out.println(j);
				System.out.println(spot[0]);
				System.out.println(spot[1]);
				System.out.println(spot[2]);
			}
		}
	}
}


		//equity barclays = new equity("barclays", 100, 0.3);
/*		
		double spot = 100;
		double strike = 100;
		double vol = 0.3;
		double drift = 0.05;
		double maturity = 365;
		double dt = maturity / (365*4);
		int nbIt = 10000;
		double finalspot = 0;
		double price = 0;
		double Poff = 0;
		Random rand = new Random(new Date().getTime());
		for (int i = 0 ; i < nbIt; i++){
			finalspot = spot;
			for (int k = 0; k < 4; k++ ){
				finalspot *= Math.exp((drift - (vol * vol) /2) * dt + vol * Math.sqrt(dt) * rand.nextGaussian());
			}
		Poff = Math.max(finalspot - strike, 0);
		price += Poff;
		}
		price /= nbIt;
		System.out.println(finalspot);
		
	}

}		
*/