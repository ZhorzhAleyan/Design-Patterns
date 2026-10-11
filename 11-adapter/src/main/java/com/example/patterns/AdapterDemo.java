package com.example.patterns;

import java.math.BigDecimal;
import java.math.RoundingMode;

public class AdapterDemo {
    interface PaymentProcessor { String pay(BigDecimal amountInDollars); }

    static class LegacyGateway {
        String chargeInCents(long cents) { return "Charged " + cents + " cents"; }
    }

    static class LegacyGatewayAdapter implements PaymentProcessor {
        private final LegacyGateway gateway;
        LegacyGatewayAdapter(LegacyGateway gateway) { this.gateway = gateway; }

        public String pay(BigDecimal amountInDollars) {
            if (amountInDollars == null || amountInDollars.signum() < 0) {
                throw new IllegalArgumentException("Amount must be non-negative");
            }
            long cents = amountInDollars.movePointRight(2)
                    .setScale(0, RoundingMode.UNNECESSARY).longValueExact();
            return gateway.chargeInCents(cents);
        }
    }

    static class Checkout {
        private final PaymentProcessor processor;
        Checkout(PaymentProcessor processor) { this.processor = processor; }
        String checkout(BigDecimal amount) { return processor.pay(amount); }
    }

    public static void main(String[] args) {
        Checkout checkout = new Checkout(new LegacyGatewayAdapter(new LegacyGateway()));
        System.out.println(checkout.checkout(new BigDecimal("10.50")));
    }
}
