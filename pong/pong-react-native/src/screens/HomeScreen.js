import React from 'react';
import {Text, View, StyleSheet, TouchableOpacity} from 'react-native';

const HomeScreen = ({navigation}) => {
    return(
        <View style={styles.background}>
            <TouchableOpacity
                onPress={() => navigation.navigate('Game')}>
                <Text style={styles.text}>Play Game!</Text>
            </TouchableOpacity>
        </View>
    )
};

const styles = StyleSheet.create({
    background: {
        flex: 1,
        alignItems: 'center',
        justifyContent: 'center',
        backgroundColor: 'orange'
    },
    text: {
        color: 'blue',
        fontWeight: 'bold',
        fontSize: 20
    }
});

export default HomeScreen;